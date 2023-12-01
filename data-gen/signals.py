import random
import math
from abc import ABC, abstractmethod
import numpy as np


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
    
    def get_freq_limits(observation_time : float) -> tuple[float, float]:
        # min freq is a quarter of full signal period
        # max freq is 5 times full signal period 
        fmin = (1 / 4) * (1 / observation_time)
        fmax = 5 * (1 / observation_time)
        return fmin, fmax

class Sine(SignalBase):

    def generate_random(
            samples : int, 
            sample_time_us : float = 1000, 
            amplitude : float = 1) -> np.ndarray:
        
        sample_time = sample_time_us / 1_000_000
        
        observation_time = samples * sample_time
        fmin, fmax = SignalBase.get_freq_limits(observation_time)
        freq = SignalBase.get_freq(fmin, fmax)
        
        signal = np.zeros(samples)
        for i in range(0, samples):
            t = i * sample_time
            sample = amplitude * math.sin(2 * math.pi * freq * t)
            signal[i] = sample
        return signal