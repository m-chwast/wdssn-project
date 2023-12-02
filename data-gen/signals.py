import random
import math
import numpy as np
from abc import ABC, abstractmethod


class SignalBase(ABC):
    
    def generate_random(
            self,
            samples : int, 
            sample_time_us : float = 1000, 
            amplitude : float = 1,
        ) -> np.ndarray:

        sample_time = sample_time_us / 1_000_000
        
        observation_time = samples * sample_time
        fmin, fmax = self.__get_freq_limits(observation_time)
        freq = self.__get_freq(fmin, fmax)        
        phase_t = (self.__get_phase() / 2 * math.pi) * (1 / freq)

        signal = np.zeros(samples)
        for i in range(0, samples):
            t = i * sample_time + phase_t
            arg = 2 * math.pi * freq * t 
                # f() is periodic with period of 2pi
            sample = amplitude * self.get_sample(arg)
            signal[i] = sample
        return signal        

    @abstractmethod
    def get_sample(self, arg : float) -> float:
        pass

    @staticmethod
    def __get_freq(fmin : float, fmax : float) -> float:
        return random.random() * (fmax - fmin) + fmin
    
    @staticmethod
    def __get_freq_limits(observation_time : float) -> tuple[float, float]:
        # min freq is a quarter of full signal period
        # max freq is 5 times full signal period 
        fmin = (1 / 4) * (1 / observation_time)
        fmax = 5 * (1 / observation_time)
        return fmin, fmax

    @staticmethod
    def __get_phase() -> float:
        return 2 * math.pi * random.random()


class Sine(SignalBase):
    
    def get_sample(self, arg : float) -> float:
        return math.sin(arg)
    

class Square(SignalBase):
    
    def get_sample(self, arg: float) -> float:
        clipped_arg = arg % (2 * math.pi)
        return 1 if clipped_arg < math.pi else 0
    