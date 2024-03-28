from ast import Dict
import time
from typing import Any, Callable, Optional
from src.model.robot import Robot
from src.connections.connection import Connection
from src.model.user import User
from src.services.users_manager import UsersManager
import matplotlib.pyplot as plt
import time


class RobotConnection(Connection):
    def __init__(self, ws: Any, manager: UsersManager) -> None:
        super().__init__(ws, manager)
    
    def create_user(self) -> User:
        return Robot()
    
    def register_command_handlers(self)-> None:
        super().register_command_handlers()
        self.command_handlers[0x05] = self.handle_receiving_frame

    def handle_receiving_frame(self, command:bytes) -> None:
        # print(f"Robot {self.user.uid} transmitted image at {time.time()} {len(command)}")
        self.user.send_message_to_linked_user(command)
    
    def loop(self):
        message = self.user.get_next_message()
        if message :
            # print(f"Robot {self.user.uid} received message: {message}")
            pass
            
        
        

        