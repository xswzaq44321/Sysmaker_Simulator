# Sysmake_QEMU

```=
cd Sysmaker_QEMU
mkdir build && cd build
../configure --target-list="arm-softmmu,arm-linux-user" --prefix=${HOME}/workspace/qemu-bin --enable-debug
make -j$(nproc) && make install
```

### Testing

- test.c
```c=
#include <stdio.h>

int main(){
    printf("Hello, ARM\n");
    return 0;
}

```

```
arm-none-linux-gnueabihf-gcc -static test.c -o test
~/workspace/qemu-bin/bin/qemu-arm test
```