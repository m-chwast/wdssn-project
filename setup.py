import os
import sys
from pathlib import Path

def utworz_wirtualne_srodowisko(venv_name = "signalVenv"):
    
    try:
        import venv
    except ImportError:
        print('Module "venv" not found')
        sys.exit(1)

    
    sciezka_venv = Path.cwd() / venv_name

    
    if sciezka_venv.exists():
        print(f"Virtual environment '{venv_name}' already exists.")
        sys.exit(1)

    
    os.system(f"python -m venv {venv_name}")

    print(f'Virtual environment "{venv_name}" has been created.')

if __name__ == "__main__":
    utworz_wirtualne_srodowisko("venv")
