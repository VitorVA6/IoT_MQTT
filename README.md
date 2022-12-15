<div id="inicio">
    <h1>PROBLEMA 3 TEC 499 Sitemas Digitas</h1>
</div>
<div id="sumario">
    <h1>Sumário</h1>
	<ul>
        	<li><a href="#equipe"> <b>Equipe de Desenvolvimento</b></li>
          	<li><a href="#introducao"> <b>Introdução</b></li>
		<li><a href="#recursos-utilizados"> <b>Recursos Utilizados</b> </a></li>
		<li><a href="#usar"> <b>Como executar</b></li>
        	<li><a href="#desenvolvimento"> <b>Descrição em alto nível do sistema proposto</b> </a> </li>
          <li><a href="#simulacao"> <b>Descrição e análise dos testes e simulações realizadas.;</b> </a> </li>
		      <li><a href="#anexos"> <b>Anexos</b> </a></li>
	</ul>	
</div>

<div id="equipe">
    <h1>Equipe de Desenvolvimento</h1>
    <ul>
		<li><a href="https://github.com/VitorVA6"> Vitor Vaz Andrade </li>
		<li><a href="https://github.com/ViniciusDJM"> Vinícius Dias de Jesus Maciel </a></li>
	</ul>
    <h1>Tutor</h1>
    <ul>
        <li><a href="https://github.com/thiagocj">Thiago Cerqueira de Jesus</a></li>
    </ul>
</div>



<div id="introducao">
     <h1> Introdução </h1>
     <ul>
	Diversos protocolos de comunicação vem sendo utilizados nos mais diversos tipos de projetos no mundo moderno, ainda mais após o interesse massivo pela Internet of Things (IOT). Com o íntuito da disciplina em nós fazer aprender a lidar com esses protocolos e saber como eles se adequam a nossa realidade, fomos apresentados ao MQTT, um protocolo que se caracteriza pela sua forma simples e eficiente de enviar e receber mensagens que se tornou muito popular em todo o mundo. Utilizando o MQTT para usar seu modelo de clientes que podem se inscrever em tópicos e assim receber e publicar informações, adaptamos o problema passado para que o mesmo possibilite a visualização das informações dos sensores para diferentes tipos de dispositivos que estejam conectados ao servidor central. 
     </ul>
</div>
<div id="recursos-utilizados">
    <h1>Recursos utilizados</h1>
    <ul>
    Para trabalhar com o nodeMCU(ESP8266) foi utilizado o Arduino IDE , para os arquivos da interface remota (HTML,CSS,Javacript) e Raspberry PI Zero usamos o Visual studio code. São remanescentes do projeto anterior, o display lcd Hitachi HD44780U para a exibição dos dados, os botões para controle do intervalo de tempo e os sensores digitais simulados por um push button e o analógico por um potenciômetro, ambos conectados ao nodeMCU, informações sobre as bibliotecas e funções que tambem foram utilizadas no problema 2 podem ser lidas no repositório do mesmo, juntamente com as informações de como configurar a IDE para poder realizar a comunicação com o nodeMCU: -REP do problema 2-
  </ul>
</div>


<div id="usar">
    <h1>Como executar o projeto</h1> 
    <h3>Rapsberry PI:</h3>
    <ul>
    Para iniciar a execução é necessário clonar o projeto da máquina que será usada. Para isso segue o passo a passo:
		
	$ https://github.com/VitorVA6/PBL2_SD
• Transfira os seguintes arquivos baixados para o Raspberry Pi Zero;

	clear.s
	writeLCD.s
	initLCD.s
	main.c
	makefile
	mapping.s

• Por fim, execute os seguintes comandos:
</ul>

	$ make
	$ sudo ./main
  
<h3>Node MCU:</h3>
    <ul>
    Para tudo funcionar bem o ambiente deve-se estar minimamente preparado, com todos os recursos citados na seção de recursos utilizados em mãos em conjunto com os códigos presentes neste repositório. Além disso é necessário a inserção de alguns dados em linhas de código específicas, pois imaginando-se o cenário de upload via wifi o código presente nos trechos do ArduinoOTA devem ficar desse modo:
    </ul>
    
    const char* ssid = "SSID-DA-REDE-A-SER-UTILIZADA";
    const char* password = "SENHA-DA-REDE-A-SER-UTILIZADA";
    
    void setup() {
	  Serial.begin(9600);
	  Serial.println("Booting");
	  WiFi.mode(WIFI_STA);
	  WiFi.begin(ssid, password);
	  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
	    Serial.println("Connection Failed! Rebooting...");
	    delay(5000);
	    ESP.restart();
	  }

	  ArduinoOTA.setHostname("ENDEREÇO-DA-ESP-A-SER-UTILIZADA");

	   ArduinoOTA.onStart([]() {
	    String type;
	    if (ArduinoOTA.getCommand() == U_FLASH) {
	      type = "sketch";
	    } else { // U_FS
	      type = "filesystem";
	    }

	  ArduinoOTA.begin();
    
</div>
<div id="desenvolvimento">
    <h1>Descrição em alto nível do sistema proposto</h1> 
    <ul>
     </ul>	
	
</div>


<div id="simulacao">
<h1> Descrição e análise dos testes e simulações realizadas</h1> 
	
</div>

<div id="anexos">
<h1> Materiais utilizados no desenvolvimento</h1> 
</div>

&nbsp;&nbsp;&nbsp;[ESP8266](https://arduino-esp8266.readthedocs.io/en/latest/)

&nbsp;&nbsp;&nbsp;[Comunicação Serial](https://www.arduino.cc/reference/en/language/functions/communication/serial/)

&nbsp;&nbsp;&nbsp;[UART - Rasp](https://raspberry-projects.com/pi/programming-in-c/uart-serial-port/using-the-uart)

</div>


<hr/>
</div>
