# Sysmaker Simulator

## Set up

1. Clone the repository (with submodules)
    ```
    git clone --recurse-submodules https://github.com/xswzaq44321/Sysmaker_Simulator.git
    ```
2. Copy the example `.env` file and configure your user-specific values (copy `.env.example` to `.env`):
    You need to fill in your user uid and user gid
    ```
    USER_UID=0    # Replace with your own `id -u`
    USER_GID=0    # Replace with your own `id -g`
    ```
    > Note: `qemu_user` inside the container will be created with the same UID and GID as your host user.
    > This is designed to avoid permission issues when accessing the mounted workspace/ volume from both your host and the container.
3. Build and run the Docker container
    ```
    docker compose up -d --build
    ```

## Attach to container

```
docker exec -it <container-name> /bin/bash
```

## Environment Details

- You will be logged in as the user `qemu_user` inside the container.
- `workspace/` is automatically mounted as a volume to `/home/qemu_user/workspace`.
- SystemC is pre-installed and located at: `/opt/systemc/`
  - `LD_LIBRARY_PATH` is set to include the SystemC library path.
  - environment varialbe `SYSTEMC_HOME` is set for locating include files easily.