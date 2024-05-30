from typing import Any
from uuid import UUID
from src.connections.connection import Connection
from src.model.person import Person
from src.model.user import User
from src.services.users_manager import UsersManager
from src.model.command_type import CommandType


class HumanConnection(Connection):
    def __init__(self, ws: Any) -> None:
        super().__init__(ws)
    
    def create_user(self) -> User:
        return Person()
    
    def set_command_handlers(self)-> None:
        super().set_command_handlers()
        self.command_handlers[CommandType.VIDEO_STREAM] = self.handle_retransmit
        self.command_handlers[CommandType.MOTOR_POWER] = self.handle_retransmit

    def handle_retransmit(self, command:bytes) -> None:
        self.user.send_message_to_linked_user(command)

    def loop(self):
        message = self.user.get_next_message()
        if message :
            self.transmit_command(message)

        