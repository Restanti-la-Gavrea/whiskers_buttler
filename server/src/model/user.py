from typing import Any, Optional
from queue import Queue,Empty,Full
from uuid import UUID
import uuid

class User:
    def __init__(self) ->None:
        self.uid:UUID = uuid.uuid4()
        self.linked_user :User = None
        self.connected:bool = False
        self.__messages:Queue[Any] = Queue(maxsize=1)
        
    def send_message(self, message:Any) -> None:
        if self.__messages.full():
            self.get_next_message()
        self.__messages.put_nowait(message)
    
    def send_message_to_linked_user(self, message:Any) -> None:
        if(self.linked_user):
            self.linked_user.send_message(message)

    def get_next_message(self) -> Any:
        try:
            return self.__messages.get_nowait()
        except Empty:
            return None
    

