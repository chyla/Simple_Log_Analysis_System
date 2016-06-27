# coding: utf-8

# -*- mode: ruby -*-
# vi: set ft=ruby :

Vagrant.configure(2) do |config|
  config.vm.box = "debian/contrib-jessie64"

  config.vm.provider "virtualbox" do |vb|
    vb.memory = "1024"
    vb.cpus = 4
  end

   config.vm.provision "shell", inline: <<-SHELL
     set -v

     sudo su -c 'echo "nameserver 8.8.8.8" > /etc/resolv.conf'

     # global archives directory
     mkdir -p /vagrant/.archives
     sudo rm -rf /var/cache/apt/archives
     sudo ln -s /vagrant/.archives /var/cache/apt/archives

     export DEBIAN_FRONTEND=noninteractive
     sudo apt-get update
     sudo apt-get upgrade -y
     sudo apt-get install -y git vim
     sudo apt-get install -y valgrind cmake autoconf automake libtool gcc g++ colorgcc sqlite3
     sudo apt-get install -y libboost-all-dev libdbus-1-dev libsqlite3-dev libcurl4-openssl-dev
     sudo apt-get install -y python-pip python-virtualenv
     sudo apt-get build-dep -y bash
     sudo pip install tox
     sudo pip install -U setuptools six

     sudo bash /vagrant/vagrant_scripts/install_gtest_gmock.sh

     echo 'export PYTHONDONTWRITEBYTECODE=1' >> /home/vagrant/.bashrc
   SHELL

   config.vm.define "server", primary: true do |server|
     server.vm.hostname = "server"
     server.vm.network "private_network", ip: "192.168.173.10"
     server.vm.network "forwarded_port", guest: 8000, host: 8000
   end

   config.vm.define "agentA" do |agentA|
     agentA.vm.hostname = "agentA"
     agentA.vm.network "private_network", ip: "192.168.173.20"
     agentA.vm.network "forwarded_port", guest: 80, host: 8081

     agentA.vm.provision "shell", inline: <<-SHELL
       set -v
       export DEBIAN_FRONTEND=noninteractive

       sudo apt-get install -y apache2 httping
     SHELL
   end

   config.vm.define "agentB" do |agentB|
     agentB.vm.hostname = "agentB"
     agentB.vm.network "private_network", ip: "192.168.173.21"
     agentB.vm.network "forwarded_port", guest: 80, host: 8082

     agentB.vm.provision "shell", inline: <<-SHELL
       set -v
       export DEBIAN_FRONTEND=noninteractive

       sudo apt-get install -y apache2 httping
     SHELL
   end

end

