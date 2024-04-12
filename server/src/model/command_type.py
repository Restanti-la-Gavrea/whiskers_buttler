from enum import Enum, auto

class CommandType(Enum):
    REGISTER = 0x00
    LINK = 0x01
    CONNECTION = 0x02
    VIDEO_STREAM = 0x04
    FRAME = 0x05
    MOTOR_POWER = 0x06
    DISTANCE_DATA = 0x07
    RING_EDGE_DATA = 0x08  

    def toBytes(self, data: bytes = None) -> bytes:
        """
        Returns the command byte, optionally prepended to the given payload.

        :param data: Optional. The payload as a bytes object to concatenate with the command byte.
        :return: A bytes object of the command byte, or the command byte plus payload if data is provided.
        """
        command_byte = self.value.to_bytes(1, byteorder='big')
        if data is not None:
            return command_byte + data
        else:
            return command_byte



    