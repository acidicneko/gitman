echo Installing GitMan...
sleep 2
make -j $(nproc)
mkdir -p ~/.local/share/gitman/bin
mkdir -p ~/.local/share/gitman/packages/gitman
mkdir -p ~/.config/gitman

cp build/gitman ~/.local/share/gitman/bin/gitman
cp assets/* ~/.local/share/gitman/
cp gitman.sh ~/.local/share/gitman/packages/gitman/
cp assets/packages.json ~/.config/gitman/

echo Generating hash...
CURRENT_HASH=$(curl -H "Accept: application/vnd.github+json" \
        https://api.github.com/repos/acidicneko/gitman/commits/main \
        | grep sha \
        | awk 'NR==1{print $2}' \
        | sed 's/[",]//g')
echo "LAST_HASH=$CURRENT_HASH" > "$HOME"/.local/share/gitman/packages/gitman/last_commit

echo Setting path variable
echo "PATH=\"\$PATH:$HOME/.local/share/gitman/bin\"" >> "$HOME"/.bashrc
echo 
echo GitMan installed!