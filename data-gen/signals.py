from abc import ABC, abstractmethod


class SignalBase(ABC):

    @abstractmethod
    def generate_random(samples : int, amplitude : float):
        pass
    

class Sine(SignalBase):

    def generate_random(samples : int, amplitude : float = 1):
        print("Generating sine")
