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
	Diversos protocolos de comunicação vêm sendo utilizados nos mais diversos tipos de projetos no mundo moderno, ainda mais após o interesse massivo pela Internet of Things (IOT). Com o intuito da disciplina em melhorar o nosso entendimento em lidar com essas ferramentas e saber como elas se adequam a nossa realidade, foi apresentado o protocolo MQTT, protocolo esse que se caracteriza pela sua forma simples e eficiente de enviar e receber mensagens que se tornou muito popular em todo o mundo. Utilizando o MQTT para usar seu modelo de clientes que podem se inscrever em tópicos e assim receber e publicar informações, adaptamos o problema passado para que o mesmo possibilite a visualização das informações dos sensores para diferentes tipos de dispositivos que estejam conectados ao servidor central. 
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
    O projeto gira em torno da interação entre nodeMCU, interface remota e rasp através do Broker:
	<div align="center">
	<img src=https://user-images.githubusercontent.com/29680023/208179943-381275b1-e11e-4091-bf24-39b55ba2ad57.png width="900px" />
	</div>
    Em nosso projeto o display LCD presente na Raspberry exibe os dados recebidos pela nodeMCU através da UART e envia os dados referentes ao intervalo de atualização, tanto a rasp, a interface remota e a nodeMCU se conectam com o broker, inicialmente realizando uma inscrição e depois publicando/recebendo atualizações no tópico.
     </ul>
     <h2>4.2  Raspberry pi zero</h2>
	<ul>
	Como dito anteriormente a Rapsberry PI utiliza seu display LCD para exibir os dados da NodeMCU, entre esses dados estão: Os valores dos sensores de D0 até D6, os dados do sensor analógico, estado da led, verificação da conexão e por fim a configuração do tempo. Os valores são exibidos de forma sequencial, e são alterados utilizando os botões (botão 1 para mudar a tela, botão 2 para incremento no tempo e botão, botão 3 para envio de atualizações).
	</ul>
	
	void screen_1(){
	    lcdClear(lcd);
	    lcdPuts(lcd, "   Sensor A0    ");
	    lcdPosition(lcd,0,1);
	    lcdPrintf(lcd, "      %sV     ", voltage);
	}

	void screen_2(){
	    lcdClear(lcd);
	    lcdPuts(lcd, " Sensores D0-D1 ");
	    lcdPosition(lcd,0,1);
	    lcdPrintf(lcd, "  D0-%s  D1-%s", d0, d1);
	}

	void screen_3(){
	    lcdClear(lcd);
	    lcdPuts(lcd, " Sensores D2-D3 ");
	    lcdPosition(lcd,0,1);
	    lcdPrintf(lcd, "  D2-%s  D3-%s", d2, d3);
	}

	void screen_4(){
	    lcdClear(lcd);
	    lcdPuts(lcd, " Sensores D4-D6 ");
	    lcdPosition(lcd,0,1);
	    lcdPrintf(lcd, "D4-%s D5-%s D6-%s", d4, d5, d6);
	}

	void screen_5(){
	    lcdClear(lcd);
	    lcdPuts(lcd, "      LED       ");
	    lcdPosition(lcd,0,1);
	    lcdPrintf(lcd, "        %s", led);
	}

	void screen_6(){
	    lcdClear(lcd);
	    lcdPuts(lcd, "Intervalo (s)");
	    lcdPosition(lcd,0,1);
	    lcdPrintf(lcd, "  %d", tempo);
	}

	void screen_7(){
	    if(status_con == 1 || status_con == 2){
		lcdClear(lcd);
		lcdPuts(lcd, "   Connection   ");
		lcdPosition(lcd,0,1);
		lcdPuts(lcd, "     ONLINE     ");
	    }    
	    else{
		lcdClear(lcd);
		lcdPuts(lcd, "   Connection   ");
		lcdPosition(lcd,0,1);
		lcdPuts(lcd, "    OFFLINE     ");
	    }
	}
<ul>
	O código acima é referente a visão de informações presentes no LCD, ao todo nos baseamos em mostrar 7 "telas" para organização de informações e cada uma das void screen são chamados em outro método a partir do incremento no Botão 1. 
</ul>
<h2>4.3  Node MCU</h2>
	
<ul>
	O Node MCU permanece com as funções contidas no Projeto 2, porém como novidade implementamos a comunicação com o Broker:
</ul>

<ul>
	Para a implementação do MQTT foi utilizada a biblioteca PubSubClient, criamos as variáveis onde estarão o endereço do servidor mqtt, junto com o usuário, senha e por fim um objeto do tipo PubSubClient :
</ul>
	
	const char* mqtt_server = "10.0.0.101";
	const char *mqtt_username = "aluno";
	const char *mqtt_password = "@luno*123";

	WiFiClient espClient;
	PubSubClient client(espClient);
	
<ul>
	Após o setup inicial, executaremos a função publishTopic, que após a conexão com o broker usando suas devidas credenciais, podemos usar o pubSubClient para realizar a comunicação com o broker. Podendo assim, escolher um tópico para se inscrever e publicar uma mensagem em seguida. Código do publishTopic:
</ul>

	  void publishTopic(){
	  float valor = analogRead(A0)*(3.3/1023.0);
	  char valorConv[4];
	  sprintf(valorConv, "%.1f", valor);
	  client.publish("VOLTAGE", valorConv);

	  int d0Value = digitalRead(D0);
	  char d0valueConv[1];
	  sprintf(d0valueConv, "%d", d0Value);
	  client.publish("D0", d0valueConv);

	  int d1Value = digitalRead(D1);
	  char d1valueConv[1];
	  sprintf(d1valueConv, "%d", d1Value);
	  client.publish("D1", d1valueConv);

	  int d2Value = digitalRead(D2);
	  char d2valueConv[1];
	  sprintf(d2valueConv, "%d", d2Value);
	  client.publish("D2", d2valueConv);

	  int d3Value = digitalRead(D3);
	  char d3valueConv[1];
	  sprintf(d3valueConv, "%d", d3Value);
	  client.publish("D3", d3valueConv);

	  int d4Value = digitalRead(D4);
	  char d4valueConv[1];
	  sprintf(d4valueConv, "%d", d4Value);
	  client.publish("D4", d4valueConv);

	  int d5Value = digitalRead(D5);
	  char d5valueConv[1];
	  sprintf(d5valueConv, "%d", d5Value);
	  client.publish("D5", d5valueConv);

	  int d6Value = digitalRead(D6);
	  char d6valueConv[1];
	  sprintf(d6valueConv, "%d", d6Value);
	  client.publish("D6", d6valueConv);
	}
	
<h2>4.4 Interface remota</h2>
<ul>
	Para a criação da interface remota foi utilizada a linguagem de marcação Python para a exibição dos dados que foram enviados pela rasp.

No arquivo main.py é onde se encontra a parte de exibição da interface remota, a mesma é dividida em 2 telas, a tela inicial é composta por 1 botão de conectar e 5 inputs, onde o usuário deverá informar: O Broker, a porta, o nome do Usuário, a Senha e por fim seu ID. 

</ul>
	<div align="center">
	<img src=https://user-images.githubusercontent.com/29680023/208192821-1cb62be9-34fe-490d-8ba6-ac6870268412.jpeg width="800px" />
	</div>
<ul>
Após a conexão ser feita, é exibida a segunda tela, onde por meios de cards o usuário receberá as informações de cada sensor digital, sensor A0, estado da LED, o tempo de envio de informações, e as últimas 10 medições. Além da visualização o usuário poderá aumentar ou diminuir o tempo de envio e ligar/desligar a led.

</ul>
	<div align="center">
	<img src=https://user-images.githubusercontent.com/29680023/208193240-af1453c0-d8e1-4e62-8d43-e3663c79f85f.jpeg width="800px" />
	</div>
	
</div>


<div id="simulacao">
<h1> Descrição e análise dos testes e simulações realizadas</h1> 
<ul>
Para a elaboração do sistema foi necessário a execução de alguns testes pontuais, como nosso sistema é baseado no conceito de unicidade de dados, usamos o app para testagem final dos valores comparando com os valores exibidos na LCD da Rasp, os testes seguem sendo detalhados a seguir:
</ul>
<ul>
	O primeiro teste foi a verificação da mudança do Sensor A0 tanto no APP, quanto na Rasp. O teste validou a eficácia do produto, garantindo a mudança de valor de forma coesa em ambas aplicações. 
</ul>
<ul>
	O segundo teste teve como foco o bom funcionamento dos sensores D0 até D6, após pressioná-los um a um e comparar com as informações obtidas no APP conseguimos garantir o bom funcionamento deste requisito. 
</ul>
<ul>
	O terceiro teste envolveu a LED, primeiro na rasp acendemos/apagamos a LED com o botão e o mesmo deveria aparecer apagado/aceso no APP, depois deveríamos alterar o estado da LED no app e garantir a sua mudança. O projeto passou no referido teste com êxito, manipulando corretamente a LED. 
</ul>
<ul>
	O quarto teste foi referente ao histórico dos dados recebidos pelo sensor analógico, sendo esse histórico montado nas últimas 10 medições. Para este teste captamos os valores do sensor e os comparamos com todo o histórico que o APP coletou, após resultados idênticos em ambas as amostras garantimos que as medições estavam corretas e responderam bem às expectativas do produto.
</ul>
<ul>
	Por fim, o quinto teste se deu pela configuração de tempo de dados para amostragem, alterando os segundos tanto na Rasp quanto no APP e aguardando em seguida para saber se a nova medição viria no tempo estipulado ou não. Após dezenas de testes com diferentes amostragens, o produto se mostrou sólido, cumprindo com êxito mais esse requisito. 
</ul>
</div>

<div id="anexos">
<h1> Materiais utilizados no desenvolvimento</h1> 
</div>

&nbsp;&nbsp;&nbsp;[Paho MQTT](https://embarcados.com.br/paho-mqtt-em-c-no-linux-embarcado/)

&nbsp;&nbsp;&nbsp;[Eclipse Paho](https://www.eclipse.org/paho/index.php?page=clients/python/docs/index.php)

&nbsp;&nbsp;&nbsp;[Paho em C](https://github.com/eclipse/paho.mqtt.c)

&nbsp;&nbsp;&nbsp;[PubSubClient](https://pubsubclient.knolleary.net/api)

&nbsp;&nbsp;&nbsp;[KivyMD](https://kivymd.readthedocs.io/en/1.1.1/)

&nbsp;&nbsp;&nbsp;[Timed Action](https://playground.arduino.cc/Code/TimedAction/)



</div>


<hr/>
</div>
