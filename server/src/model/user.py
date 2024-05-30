from typing import Any, Optional
from queue import Queue,Empty,Full
from uuid import UUID
import uuid

class User:
    def __init__(self) ->None:
        self.uid:UUID = uuid.uuid4()
        self.linkedUser :User = None
        self.connected:bool = False
        self.__messages:Queue[bytes] = Queue(maxsize=1)
        
    def send_message(self, message:bytes) -> None:
        if self.__messages.full():
            self.get_next_message()
        self.__messages.put_nowait(message)
    
    def send_message_to_linked_user(self, message:bytes) -> None:
        if(self.linkedUser):
            self.linkedUser.send_message(message)

    def get_next_message(self) -> Optional[bytes]:
        try:
            return self.__messages.get_nowait()
        except Empty:
            return None
        
    def getName(self)->str:
        return f"User {self.uid}"
    

