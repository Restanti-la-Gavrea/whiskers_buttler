from typing import Any
from uuid import UUID
from src.connections.connection import Connection
from src.model.person import Person
from src.model.user import User
from src.services.users_manager import UsersManager
from src.model.command_type import CommandType


class PersonConnection(Connection):
    def __init__(self, ws: Any, manager: UsersManager) -> None:
        super().__init__(ws, manager)
    
    def create_user(self) -> User:
        return Person()
    
    def register_command_handlers(self)-> None:
        super().register_command_handlers()
        self.command_handlers[CommandType.VIDEO_STREAM] = self.handle_retransmit

    def handle_retransmit(self, command:bytes) -> None:
        self.user.send_message_to_linked_user(command)

    def loop(self):
        message = self.user.get_next_message()
        if message :
            self.transmit_command(message)

        