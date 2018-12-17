export PREFIX="$PWD/toolchain/"
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"

echo $PREFIX/src
cd $PREFIX/src
mkdir build-binutils
cd build-binutils
../binutils-2.31/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
make
make install

cd $PREFIX/src
# The $PREFIX/bin dir _must_ be in the PATH. We did that above.
which -- $TARGET-as || echo $TARGET-as is not in the PATH
 
mkdir build-gcc
cd build-gcc
../gcc-8.2.0/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers
make all-gcc
make all-target-libgcc
make install-gcc
make install-target-libgcc