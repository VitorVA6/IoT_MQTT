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
     <h1>1. Introdução </h1>
     <ul>
	Diversos protocolos de comunicação vem sendo utilizados nos mais diversos tipos de projetos no mundo moderno, ainda mais após o interesse massivo pela Internet of Things (IOT). Com o íntuito da disciplina em nós fazer aprender a lidar com esses protocolos e saber como eles se adequam a nossa realidade, foi apresentado o protocolo MQTT, protocolo esse que se caracteriza pela sua forma simples e eficiente de enviar e receber mensagens que se tornou muito popular em todo o mundo. Utilizando o MQTT para usar seu modelo de clientes que podem se inscrever em tópicos e assim receber e publicar informações, adaptamos o problema passado para que o mesmo possibilite a visualização das informações dos sensores para diferentes tipos de dispositivos que estejam conectados ao servidor central. 
     </ul>
</div>
<div id="recursos-utilizados">
    <h1>2. Recursos utilizados</h1>
    <ul>
    Para trabalhar com o nodeMCU(ESP8266) foi utilizado o Arduino IDE , para os arquivos da interface remota (Python) e Raspberry PI Zero usamos o Visual studio code. 
    </ul>
    <ul>
    Permanecem do projeto anterior o mesmo display lcd Hitachi HD44780U para a exibição dos dados, os botões para controle do intervalo de tempo e os sensores digitais simulados por um push button e o analógico por um potenciômetro, ambos conectados ao nodeMCU. 
    </ul>
    <ul>
    Para reaver as informações sobre funções e bibliotecas que também foram utilizadas no problema 2, deve-se acessar o seguinte repositório: https://github.com/VitorVA6/PBL2_SD.  
  </ul>
</div>


<div id="usar">
    <h1>3. Como executar o projeto</h1> 
    <ul>
    Para iniciar a execução é necessário clonar o projeto da máquina que será usada. Para isso segue o passo a passo:
		
	$https://github.com/VitorVA6/IoT_MQTT
• Transfira a seguinte pasta baixada para o Raspberry Pi Zero;

	rasp_mqtt
	
• Para a parte destinada ao NodeMCU deve ser compilada usando o Arduino, logo deve-se baixar seguinte pasta;
	
	esp_mqtt
	
• Para o executável baixe os arquivos presentes na pasta app_mqtt e execute o Main em uma IDE de sua preferência;

• Por fim, execute os seguintes comandos:
	
	$ make
	$ sudo ./main

</ul>

      
</div>
<div id="desenvolvimento">
    <h1>4. Descrição em alto nível do sistema proposto</h1> 
    <h2>4.1 Fluxo de funcionamento do projeto</h2> 
    <ul>
    O projeto gira em torno da interação entre nodeMCU, interface remota e rasp atráves do Broker :
	<div align="center">
	<img src=https://user-images.githubusercontent.com/29680023/208174728-76f637f4-5d7c-4d3b-9b02-db2a9d896ca4.png width="900px" />
	</div>
    Em nosso projeto o display LCD presente na Raspberry exibe os dados recebidos pela nodeMCU através da UART e envia os dados referentes ao intervalo de atualização, tanto a rasp, a interface remota e a nodeMCU se conectam com o broker, inicialmente realizando uma inscricão e depois publicando/recebendo atualizações no tópico.
     </ul>
     <h2>4.2  NodeMCU (ESP8266)</h2>
     
	
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
