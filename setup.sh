if [ -f /etc/os-release ]; then
	. /etc/os-release
	
	if [ $ID == "arch" ]; then
		echo "Installing dependencies"
		sudo pacman -S libmicrohttpd jansson curl gnutls libgcrypt
	elif [ $ID == "debian" ]; then
		echo "Installing dependencies"
		sudo apt-get install libmicrohttpd-dev libjansson-dev libcurl4-gnutls-dev libgnutls28-dev libgcrypt20-dev
	else
		echo "Sorry, don't know how to set up dependencies. Please install your distro's equivalent of debian's libmicrohttpd-dev libjansson-dev libcurl4-gnutls-dev libgnutls28-dev libgcrypt20-dev, then hit enter to continue."
		read
	fi

	git clone https://github.com/babelouest/ulfius.git
	cd ulfius
	git submodule update --init
	cd lib/orcania
	make
	echo -e "\nInstalling orcania.\n"
	sudo make install
	cd ../yder
	make
	echo -e "\nInstalling yder.\n"
	if ! sudo make install ; then
		echo -e "\nyder install failed. This may be due to the version number in orcania. Try symlinking /usr/local/lib/liborcania.so to /usr/local/lib/liborcania.so.(version)."
		echo "Once that's done, run this script again."
		exit 1
	fi
	cd ../.. &&	make && sudo make install
	cd .. && rm -rf ulfius
fi
