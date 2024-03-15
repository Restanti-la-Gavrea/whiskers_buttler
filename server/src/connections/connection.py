from typing import Any, Optional
from queue import Queue,Empty

from src.services.users_manager import UsersManager
from src.model.user import User

import threading

class Connection:
    def __init__(self, ws:Any, manager: UsersManager) -> None:
        self.ws:Any = ws
        self.manager: UsersManager = manager
        self.user:Optional[User] = None
        self.receiving_commands_thread: Optional[threading.Thread] = None
    
    def start_loop_conection(self) -> None:
        try:
            self.on_start()
            while self.ws.connected:
                self.loop()
        except Exception:
            pass
        finally:
            self.on_end()
    
    def waiting_for_commands(self):
        try:
            while self.ws.connected:
                command = self.ws.receive()
                if command:
                    self.on_receive_command(command)
        except Exception as e:
            pass
    
    def transmit_command(self, command:Any):
        return self.ws.send(command)


    def on_start(self) -> None:
        self.user = self.create_user()
        self.manager.add_user(self.user)
        self.manager.connect_user(self.user.uid)
        self.isRunning = True
        self.receiving_commands_thread = threading.Thread(target=self.waiting_for_commands)
        self.receiving_commands_thread.start()
    
    def on_end(self) -> None:
        if not self.ws.connected :
            self.ws.close()
        self.receiving_commands_thread.join()
        self.manager.disconnect_user(self.user.uid)
    
    def loop(self):
        pass

    def on_receive_command(self,command : Any)-> None:
        pass

    def create_user() -> User:
        pass
    
