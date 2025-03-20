# Sysmake_QEMU

```=
cd Sysmaker_QEMU
mkdir build && cd build
../configure --target-list="arm-softmmu,arm-linux-user" --prefix=${HOME}/workspace/qemu-bin
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
arm-linux-gnueabi-gcc test.c -o test.o
~/workspace/qemu-bin/bin/qemu-arm -L /usr/arm-linux-gnueabi/ test.o
```