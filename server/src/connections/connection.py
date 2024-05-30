from ast import Dict
import time
from typing import Any, Callable, Optional
from queue import Queue,Empty
import uuid
from uuid import UUID

from src.services.users_manager import UsersManager
from src.model.user import User
from src.model.command_type import CommandType
import threading

class Connection:
    __lock:threading.Lock = threading.Lock()

    def __init__(self, ws:Any) -> None:
        self.ws:Any = ws
        self.user:Optional[User] = None
        self.receiving_commands_thread: Optional[threading.Thread] = None
        self.command_handlers: Dict[CommandType, Callable[[Optional[bytes]], None]] = {}
    
    def start_loop_conection(self) -> None:
        try:
            self.on_start()
            while self.ws.connected:
                if self.user:
                    self.__lock.acquire()
                    try:
                        self.loop()
                    finally:
                        self.__lock.release()
                else :
                    # if the user is not authentificated sleep 250 ms
                    time.sleep(0.25)
        except Exception as e:
            print(e)
            pass
        finally:
            self.closeConnection()
            self.on_end()
    
    def on_start(self) -> None:
        self.set_command_handlers()
        self.receiving_commands_thread = threading.Thread(target=self.waiting_for_commands)
        self.receiving_commands_thread.start()
        
    def set_command_handlers(self) -> None:
        self.command_handlers[CommandType.REGISTER] = self.handle_register_or_authenticate
        self.command_handlers[CommandType.LINK] = self.handle_linkage_attempt

    def waiting_for_commands(self):
        # Waiting for commands thread fucntion
        try:
            while self.ws.connected:
                command = self.ws.receive(timeout = 0.5)
                if command:
                    try:
                        commandType = CommandType(command[0])
                        self.__lock.acquire()
                        try:
                            self.command_handlers[commandType](command)
                        finally:
                            self.__lock.release()
                    except Exception as e:
                        self.handle_unknown_command(command)
        except Exception as e:
            print(e)
            pass
        finally:
            self.closeConnection()
    
    def transmit_command(self, command:bytes):
        return self.ws.send(command)

    def closeConnection(self) -> None:
        try:
            self.ws.close()
        except Exception as e:
            pass
    
    def loop(self):
        pass

    def on_end(self) -> None:
        self.closeConnection()
        if self.user is not None:
            self.__lock.acquire()
            logOutMessage = CommandType.CONNECTION.toBytes(b'\x00')
            self.user.send_message_to_linked_user(logOutMessage)
            UsersManager().disconnect_user(self.user.uid)
            self.__lock.release()
        self.receiving_commands_thread.join()

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
        user = UsersManager().get_user(uuid)
        if user:
            self.user = user
            UsersManager().connect_user(user.uid)
            return True
        return False
    
    def register(self)->None:
        self.user = self.create_user()
        UsersManager().add_user(self.user)
        UsersManager().connect_user(self.user.uid)
        
    def handle_register_or_authenticate(self, command: bytes)->None:
        if self.user is None:
            if self.authentification(command[1:]):
                loginMessage = CommandType.CONNECTION.toBytes(b'\x01')
                self.user.send_message_to_linked_user(loginMessage)
            else:
                self.register()
        uuid_bytes = self.user.uid.bytes
        response_command = CommandType.REGISTER.toBytes(uuid_bytes)
        self.transmit_command(response_command)

    def handle_linkage_attempt(self, command: bytes)->None:
        transmitted_uuid = self.bytes_to_uuid(command[1:])
        if transmitted_uuid is not None and self.user is not None:
            if UsersManager().link_user(self.user.uid,transmitted_uuid):
                # Transmit succesfull linkage
                self.transmit_command(command)
                return
        self.transmit_command(CommandType.LINK.toBytes())
        
    def handle_unknown_command(self, command: bytes):
        print(f"Unknown command {command[0]}")

    def create_user() -> User:
        pass
    
