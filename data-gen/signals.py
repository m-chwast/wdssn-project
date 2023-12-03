import random
import math
import numpy as np
from abc import ABC, abstractmethod
from scipy.signal import square


class SignalBase(ABC):
    
    def generate_random(
            self,
            samples : int, 
            sample_time_us : float = 1000, 
            amplitude : float = 1,
            noise_percent : float = 10
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
            sample = amplitude * self.get_sample(arg)
            sample += self.__get_noise(amplitude, noise_percent)
            signal[i] = sample
        return signal        

    # get_sample() is periodic with period of 2pi
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

    @staticmethod
    def __get_noise(amplitude : float, noise_percent : float) -> float:
        return amplitude * (noise_percent / 100) * 2 * (random.random() - 0.5)


class Sine(SignalBase):
    
    def get_sample(self, arg : float) -> float:
        return math.sin(arg)
    

class Square(SignalBase):
    
    def get_sample(self, arg: float) -> float:
        return square(t=arg, duty=0.5)
    