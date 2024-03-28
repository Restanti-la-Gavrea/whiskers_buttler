from src.model.user import User


class Person(User):
    def __init__(self) ->None:
        super().__init__()
    
    def getName(self) -> str:
        return f"Person {self.uid}"