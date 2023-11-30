import random
import math
from abc import ABC, abstractmethod


class SignalBase(ABC):

    @abstractmethod
    def generate_random(
        samples : int, 
        fmin : float, fmax : float, 
        sample_time_us : float,
        amplitude : float):
        pass

    def get_freq(fmin : float, fmax : float) -> float:
        return random.random() * (fmax - fmin) + fmin
    

class Sine(SignalBase):

    def generate_random(
            samples : int, 
            fmin : float = 10, fmax : float = 1000,
            sample_time_us : float = 1000, 
            amplitude : float = 1):
        print("Generating sine")

        freq = SignalBase.get_freq(fmin, fmax)
        for i in range(0, samples):
            t = i * sample_time_us / 1_000_000
            sample = amplitude * math.sin(2 * math.pi * freq * t)
            print(sample)