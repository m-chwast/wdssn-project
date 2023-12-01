import random
import math
from typing import Callable
import numpy as np


class SignalBase:

    @staticmethod
    def generate_random(
            samples : int, 
            sample_time_us : float, 
            amplitude : float,
            f : Callable[[float], float]
        ) -> np.ndarray:

        sample_time = sample_time_us / 1_000_000
        
        observation_time = samples * sample_time
        fmin, fmax = SignalBase.get_freq_limits(observation_time)
        freq = SignalBase.get_freq(fmin, fmax)        
        phase_t = (SignalBase.get_phase() / 2 * math.pi) * (1 / freq)

        signal = np.zeros(samples)
        for i in range(0, samples):
            t = i * sample_time + phase_t
            # it's assumed that provided function is periodic with period of 2pi
            arg = 2 * math.pi * freq * t    
            sample = amplitude * f(arg)
            signal[i] = sample
        return signal        


    @staticmethod
    def get_freq(fmin : float, fmax : float) -> float:
        return random.random() * (fmax - fmin) + fmin
    
    @staticmethod
    def get_freq_limits(observation_time : float) -> tuple[float, float]:
        # min freq is a quarter of full signal period
        # max freq is 5 times full signal period 
        fmin = (1 / 4) * (1 / observation_time)
        fmax = 5 * (1 / observation_time)
        return fmin, fmax

    @staticmethod
    def get_phase() -> float:
        return 2 * math.pi * random.random()


class Sine(SignalBase):

    @staticmethod
    def generate_random(
            samples : int, 
            sample_time_us : float = 1000, 
            amplitude : float = 1) -> np.ndarray:
        
        return SignalBase.generate_random(
            samples, 
            sample_time_us, 
            amplitude, 
            Sine.get_sample)
    
    @staticmethod
    def get_sample(arg) -> float:
        return math.sin(arg)
    