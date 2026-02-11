Cross-complilation Ways
1 . Step 1 : download Builtroot
wget https://buildroot.org/downloads/buildroot-2024.02.3.tar.gz
tar -xvf buildroot-2024.02.3.tar.gz
cd buildroot-2024.02.3.tar.gz
make menuconfig

NOTICE:config linux kenel ,need config c++ support.
# 使用所有可用CPU核心
make -j$(nproc)      # Linux

if complilation success , exist output dir

hugohu@hugohu:~/Desktop/linuxsystem/qemu-buildroot/buildroot-2024.02.3/output$ ls
build  host  images  staging  target


2 . Step 2 : 
sh build_arm.sh
notice : Compile code through CMakeLists, mount rootfs.ext2  DIR, COPY Compile SUCCESS code TO rootfs.ext2

3. step 3 :
sh run_qumu.sh
Compile code after in image