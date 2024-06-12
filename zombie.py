import psutil

def find_zombie_processes():
    # List to store information about zombie processes
    zombie_processes = []

    # Iterate over all running processes
    for proc in psutil.process_iter(['pid', 'name', 'status']):
        try:
            # Check if the process status is 'zombie'
            if proc.info['status'] == psutil.STATUS_ZOMBIE:
                zombie_processes.append(proc.info)
        except (psutil.NoSuchProcess, psutil.AccessDenied, psutil.ZombieProcess):
            pass

    return zombie_processes

def main():
    zombie_processes = find_zombie_processes()

    if zombie_processes:
        print("Zombie processes found:")
        for zombie in zombie_processes:
            print(f"PID: {zombie['pid']}, Name: {zombie['name']}")
    else:
        print("No zombie processes found.")

if __name__ == "__main__":
    main()
