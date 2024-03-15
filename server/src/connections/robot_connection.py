import time
from typing import Any
import uuid
from src.model.robot import Robot
from src.connections.connection import Connection
from src.model.user import User
from src.services.users_manager import UsersManager
import json
import numpy as np
import matplotlib.pyplot as plt


class RobotConnection(Connection):
    def __init__(self, ws: Any, manager: UsersManager) -> None:
        super().__init__(ws, manager)
    
    def create_user(self) -> User:
        return Robot()
    
    def on_receive_command(self,command: Any) -> None:
        print(f"Robot {self.user.uid} transmitted image at {time.time()}")
        self.user.send_message_to_linked_user(command)
    
    def loop(self):
        while True:
            message = self.user.get_next_message()
            if message :
                print(f"Robot {self.user.uid} received message: {message}")
            else:
                break
            
        
        

        