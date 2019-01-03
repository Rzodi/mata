instalacja i konfiguracja apacha
sudo apt-get install apache2
edytujemy plik /etc/apache2/apache2.conf dodajemy to : 

<Directory /var/www/>
        Options Indexes FollowSymLinks ExecCGI
        AllowOverride None
        Require all granted
        AddHandler cgi-script .cgi
</Directory>

inne wpisy zawierające <Directory /var/www/> raczej komentujemy

cd /etc/apache2/mods-enabled
sudo ln -s ../mods-available/cgi.load
sudo service apache2 reload

Apache już jest, teraz do folderu /var/www/html/ kopijemy zawartość folderu z projektem

sudo chmod -R 757 /var/www

przechodzimy do folderu /var/www/html i tam: 
sudo chmod 777 solve.cgi
sudo service apache2 reload
sudo systemctl restart apache2.service

wchodzimy w przeglądarkę na 127.0.0.1 optymalizator pownien działać ! 




