from typing import Any
from uuid import UUID
from src.connections.connection import Connection
from src.model.person import Person
from src.model.user import User
from src.services.users_manager import UsersManager
import json
import time


class PersonConnection(Connection):
    def __init__(self, ws: Any, manager: UsersManager) -> None:
        super().__init__(ws, manager)
    
    def create_user(self) -> User:
        return Person()
    
    def on_receive_command(self,command: Any) -> None:
        commandJs = json.loads(command)
        if "connect-to" in commandJs:
            uuid = UUID(commandJs['connect-to'])
            self.manager.link_user(self.user.uid, uuid)
        else:
            print(f"Person {self.user.uid} transmitted message: {commandJs}")
    
    def loop(self):
        # Process messages received by the user
        while True:
            message = self.user.get_next_message()
            if message :
                print(f"Person {self.user.uid} received message at {time.time()}")
                self.transmit_command(message)
            else:
                break

        