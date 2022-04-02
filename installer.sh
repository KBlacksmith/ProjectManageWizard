wget https://raw.githubusercontent.com/KBlacksmith/ProjectManageWizard/main/main.cpp
g++ -v -std=c++2a main.cpp -o wizard
rm main.cpp

location=$(pwd)
user=$(whoami)

echo "export PATH=\"\$PATH:$location\"" >> /home/$user/.bashrc

