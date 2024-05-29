#!/bin/bash
install() {
	make -j $(nproc)
	mkdir -p ~/.local/share/gitman/bin
	mkdir -p ~/.local/share/gitman/packages/gitman
	mkdir -p ~/.config/gitman

	cp build/gitman ~/.local/share/gitman/bin/gitman
	cp assets/* ~/.local/share/gitman/
	cp gitman.sh ~/.local/share/gitman/packages/gitman/
	cp assets/packages.json ~/.config/gitman/
}

uninstall() {
	rm ~/.local/share/gitman/bin/gitman
}

update(){
	make -j $(nproc)
	mv build/gitman ~/.local/share/gitman/bin/gitman
	cp assets/* ~/.local/share/gitman/
}

if [ $1 = "install" ] ; then 
	install
elif [ $1 = "uninstall" ] ; then
	uninstall
elif [ $1 = "update" ] ; then
	update
else
	echo unknown option
	exit 1
fi