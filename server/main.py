import logging
import time
import serial
from contextlib import asynccontextmanager
from fastapi import FastAPI, HTTPException
from pydantic import BaseModel

logging.basicConfig(
    level=logging.DEBUG,
    format="%(asctime)s [%(levelname)s] %(message)s",
)
log = logging.getLogger(__name__)

SERIAL_PORT = "/dev/ttyUSB0"
BAUD_RATE = 9600

ser: serial.Serial = None


@asynccontextmanager
async def lifespan(app: FastAPI):
    global ser
    log.info(f"opening serial port {SERIAL_PORT}")
    ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=2)
    time.sleep(2)  # wait for Arduino reboot after DTR reset
    ser.reset_input_buffer()
    log.info("serial ready")
    yield
    ser.close()
    log.info("serial closed")


app = FastAPI(lifespan=lifespan)


class SetTempMode(BaseModel):
    mode: str

class SetLcdScreen(BaseModel):
    msg: str


def scpi(cmd: bytes) -> str:
    log.debug(f"sending: {cmd!r}")
    ser.reset_input_buffer()
    ser.write(cmd)
    raw = ser.readline()
    log.debug(f"received: {raw!r}")
    return raw.decode().strip()


@app.get("/temperature")
def read_temperature():
    log.info("GET /temperature")
    response = scpi(b"MEAS:TEMP?\n")
    if not response:
        raise HTTPException(status_code=504, detail="No response from device")
    return {"temperature": response}


@app.get("/id")
def get_device_id():
    log.info("GET /id")
    response = scpi(b"*IND\n")
    if not response:
        raise HTTPException(status_code=504, detail="No response from device")
    return {"id": response}


@app.post("/temperature/mode")
def set_temp_mode(req: SetTempMode):
    log.info("POST /temperature/mode")
    response = scpi(f"CONF:TEMP {req.mode}\n".encode())
    if not response:
        raise HTTPException(status_code=504, detail="No response from device")
    return {"result": response}

@app.post("/lcd/screen")
def set_lcd(req: SetLcdScreen):
    log.info("POST /lcd/screen")
    response = scpi(f"LCD:TEXT {req.msg}\n".encode())
    if not response:
        raise HTTPException(status_code=504, detail="No response from device")
    return {"result": response}
