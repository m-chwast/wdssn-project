# Signal type classifier

## Setup environment
- run `setup.py` to create virtual environment
- activate environment with generated script in _{venv name}/Scripts/activate_
- then install requred modules with `pip install -r requirements.txt` comment

## Data setup
- Generating data:
    - enter _data-gen_ directory
    - generate the data by running _data-gen.py_ with uncommented _generate()_ function in main
- Running IA:
    - enter _network_ directory
    - run _network.py_

### Creating your own Pattern
If you have an idea to expande model with new, more deficult signals, all you have to do is to create a new class that will inherits from `SignalBase` and override `get_sample(self, arg: float) -> float` method.
