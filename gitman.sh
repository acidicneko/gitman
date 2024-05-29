#!/bin/bash
install() {
	make -j $(nproc)
    mkdir -p ~/.local/share/gitman/bin
    mkdir -p ~/.local/share/gitman/packages/gitman

    cp build/gitman ~/.local/share/gitman/bin/gitman
    cp assets/* ~/.local/share/gitman/
}

uninstall() {
	rm ~/.local/share/gitman/bin/gitman
}

update(){
	install
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