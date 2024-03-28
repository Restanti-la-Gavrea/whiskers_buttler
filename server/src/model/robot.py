from src.model.user import User


class Robot(User):
    def __init__(self) -> None:
        super().__init__()

    def getName(self) -> str:
        return f"Robot {self.uid}"