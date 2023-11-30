from abc import ABC, abstractmethod


class SignalBase(ABC):

    @abstractmethod
    def generate_random(
        samples : int, 
        fmin : float, fmax : float, 
        sample_time_us : float,
        amplitude : float):
        pass
    

class Sine(SignalBase):

    def generate_random(
            samples : int, 
            fmin : float = 10, fmax : float = 1000,
            sample_time_us : float = 1000, 
            amplitude : float = 1):
        print("Generating sine")

