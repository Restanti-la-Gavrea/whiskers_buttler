from typing import Dict, Optional, List
from src.model.robot import Robot
from src.model.user import User
from uuid import UUID

class UsersManager:
    def __init__(self) -> None:
        self.__users: Dict[UUID, User] = {}
    
    def get_all_users(self)->List[User]:
        return self.__users.items

    def get_all_connected_robots(self)->List[Robot]:
        return [robot for robot in self.__users.values() if isinstance(robot, Robot) and robot.connected]

    def add_user(self, user:User) -> None:
        self.__users[user.uid] = user

    def get_user(self, uid:UUID) -> Optional[User]:
        return self.__users.get(uid)
    
    def delete_user(self, uid:UUID) -> None:
        user = self.get_user(uid)
        if user :
            print(f"{user.getName()} deleted")
            linked_user_uid = user.linked_user
            del self.__users[uid]
            self.unlink_user(linked_user_uid)

    def connect_user(self, uid:UUID) -> None:
        user = self.get_user(uid)
        if user:
            print(f"{user.getName()} connected")
            user.connected = True

    def disconnect_user(self, uid:UUID) -> None:
        user = self.get_user(uid)
        print(f"{user.getName()} disconnected")
        if user:
            if user.linked_user:
                if not user.linked_user.connected:
                    self.delete_user(user.linked_user.uid)
                    self.delete_user(user.uid)
                else:
                    user.connected = False
            else:
                self.delete_user(user.uid)
            user.connected = False
    
    def link_user(self, uid1:UUID, uid2:UUID) -> None:
        user1 = self.get_user(uid1)
        user2 = self.get_user(uid2)
        if user1 and user2:
            self.unlink_user(uid1)
            self.unlink_user(uid2)
            user1.linked_user = user2
            user2.linked_user = user1
            print(f"{user1.getName()} and {user2.getName()} are linked")
            return True
        return False
        
    def unlink_user(self, uid:UUID) -> None:
        user = self.get_user(uid)
        if user :
            linked_user = user.linked_user
            if linked_user :
                user.linked_user = None
                if linked_user.connected :
                    linked_user.linked_user = None
                else:
                    self.delete_user(linked_user.uid)
                print(f"{user.getName()} and {linked_user.getName()} are unlinked") 
