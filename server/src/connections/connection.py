from ast import Dict
import time
from typing import Any, Callable, Optional
from queue import Queue,Empty
import uuid
from uuid import UUID

from src.services.users_manager import UsersManager
from src.model.user import User
import threading

class Connection:
    def __init__(self, ws:Any, manager: UsersManager) -> None:
        self.ws:Any = ws
        self.manager: UsersManager = manager
        self.user:Optional[User] = None
        self.receiving_commands_thread: Optional[threading.Thread] = None
        self.command_handlers: Dict[int, Callable[[Optional[bytes]], None]] = {}
        self.register_command_handlers()
    
    def register_command_handlers(self) -> None:
        self.command_handlers[0x00] = self.handle_register_or_authenticate
        self.command_handlers[0x01] = self.handle_linkage_attempt
        self.default_handler = self.handle_unknown_command
    
    def start_loop_conection(self) -> None:
        try:
            self.on_start()
            while self.ws.connected:
                if self.user:
                    self.loop()
                else :
                    # if the user is not authentificated sleep 500 ms
                    time.sleep(0.5)
        except Exception:
            pass
        finally:
            self.on_end()
    
    def waiting_for_commands(self):
        # Waiting for commands thread fucntion
        try:
            while self.ws.connected:
                command = self.ws.receive(timeout = 4)
                if command:
                    self.on_receive_command(command)
        except Exception as e:
            pass
        finally:
            self.closeConnection()
    
    def transmit_command(self, command:bytes):
        return self.ws.send(command)

    def on_start(self) -> None:
        self.receiving_commands_thread = threading.Thread(target=self.waiting_for_commands)
        self.receiving_commands_thread.start()
    
    def on_end(self) -> None:
        self.closeConnection()
        if self.user is not None:
            self.manager.disconnect_user(self.user.uid)
        self.receiving_commands_thread.join()

    def closeConnection(self) -> None:
        try:
            self.ws.close()
        except Exception as e:
            pass
    
    def loop(self):
        pass

    def on_receive_command(self,command : bytes)-> None:
        # print(f"command : {command[0]}")
        # print(self.command_handlers[command[0]])
        self.command_handlers[command[0]](command)

    @staticmethod
    def bytes_to_uuid(uuid_bytes:bytes)->Optional[UUID]:
        if len(uuid_bytes) != 16:
            return None
        try :
            return UUID(bytes=uuid_bytes)
        except Exception:
            return None

    def authentification(self, uuid_bytes:bytes)->bool:
        uuid = self.bytes_to_uuid(uuid_bytes)
        
        if uuid is None:
            return False
        user = self.manager.get_user(uuid)
        if user:
            self.user = user
            self.manager.connect_user(user.uid)
            return True
        return False
    
    def register(self)->None:
        self.user = self.create_user()
        self.manager.add_user(self.user)
        self.manager.connect_user(self.user.uid)
        
    def handle_register_or_authenticate(self, command: bytes)->None:
        if self.authentification(command[1:]) == False:
            self.register()
        uuid_bytes = self.user.uid.bytes
        response_command = command[0:1] + uuid_bytes
        self.transmit_command(response_command)

    def handle_linkage_attempt(self, command: bytes)->None:
        transmitted_uuid = self.bytes_to_uuid(command[1:])
        if transmitted_uuid is not None and self.user is not None:
            self.manager.link_user(self.user.uid,transmitted_uuid)
        
    def handle_unknown_command(self, command: bytes):
        pass

    def create_user() -> User:
        pass
    
