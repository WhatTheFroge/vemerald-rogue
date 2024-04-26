echo Working Directory: $PWD

echo Running Test Build.. [make -j$(nproc) -O pokeemerald-test.elf TEST=1 RELEASE=0]
make -j$(nproc) -O pokeemerald-test.elf TEST=1 RELEASE=0

exitCode=$?

# Can comment this out once bug with log disappearing is fixed
if [ $exitCode != 0 ]
then
    echo Error! [Exit code $exitCode]
    read -n 1 -s -r -p "Press any key to continue..."
else
    echo Success!
fi

exit $exitCode