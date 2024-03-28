from typing import Any
from uuid import UUID
from src.connections.connection import Connection
from src.model.person import Person
from src.model.user import User
from src.services.users_manager import UsersManager


class PersonConnection(Connection):
    def __init__(self, ws: Any, manager: UsersManager) -> None:
        super().__init__(ws, manager)
    
    def create_user(self) -> User:
        return Person()
    
    def register_command_handlers(self)-> None:
        super().register_command_handlers()
        # self.command_handlers[0x05] = self.handle_receiving_frame

    def loop(self):
        message = self.user.get_next_message()
        if message :
            # print(f"Person {self.user.uid} received message at {time.time()}")
            self.transmit_command(message)

        