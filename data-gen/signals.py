from abc import ABC, abstractmethod


class SignalBase(ABC):

    @abstractmethod
    def generate_random():
        pass
    

class Sine(SignalBase):

    def generate_random():
        print("Generating sine")
