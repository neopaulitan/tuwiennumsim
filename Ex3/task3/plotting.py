import subprocess
import matplotlib.pyplot as plt
import os

def run_mc_int(func, num_threads):
    # Set the environment variable
    env = os.environ.copy()
    env['OMP_NUM_THREADS'] = str(num_threads)

    # Run the command with the environment variable
    result = subprocess.run(['./mc_int', func, '-1.57079632679', '1.57079632679', '1000000'], 
                            capture_output=True, text=True, env=env)
    
    # Extract the line containing "Elapsed time:"
    output = result.stdout.strip()
    runtime = next(line for line in output.split('\n') if "Elapsed time:" in line)

    print(runtime)

    runtime = float(runtime.split(':')[1].strip().removesuffix('s'))
    
    return runtime

if __name__ == "__main__":
    functions = ['sinx', 'cos2xinv', 'x4m5']
    num_threads = [1, 5, 10, 20, 40, 80]

    runtimes = {func: [run_mc_int(func, num_thread) for num_thread in num_threads] for func in functions}

    plt.switch_backend('TkAgg')
    for func in functions:
        plt.scatter(num_threads, runtimes[func], label=func)

    plt.xlabel('Number of threads')
    plt.ylabel('Runtime (s)')
    plt.legend()
    plt.show()