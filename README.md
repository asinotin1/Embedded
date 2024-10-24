<details>
  <summary>LESSON 1: GPIO</summary>
  Để sử dụng ngoại vi GPIO ta cần thực hiện các bước sau :
  
  ![image](https://github.com/user-attachments/assets/0c621346-dd76-4e1b-be1c-ae0e3be759b1)
  
 * B1: Cấp xung clock cho ngoại vi
  Ta cần phải đọc vào diagram của mỗi loại để biết được cần phải cấp xung clock cho bus nào trên ngoại vi
  dưới đây là diagram của dòng stm32f103
  
  ![image](https://github.com/user-attachments/assets/6064d888-84fb-4ada-9d8f-b8004c4eab84)

Ví dụ: nếu bạn muốn sử dụng chân PA2 thì bạn phải cấp xung cho GPIO, muốn cấp xung cho GPIO thì phải cấp xung cho bus mà GPIO treo lên là APB2.

  * Trong VDK STM32 thì thanh ghi dùng để cấp clock là thanh ghi RCC

  * Mình sử dụng thư viện chuẩn, cú pháp để cấp/không cấp clock cho một ngoại vi là: RCC_@PeriphClockCmd(A, B) , với @ là tên bus mà mình muốn cấp clock(AHB, APB1, APB2), A là ngoại vi mà mình muốn cấp clock(RCC_APB2Periph_GPIOC,....), B là cho phép cấp/không cấp (ENABLE,...).
  
* B2 : Cấu hình ngoại vi cho GPIO
  cấu hình cho GPIO có 3 thông số chính là : pin, mode, speed.
    * Pin : Là chân mình muốn dùng (0->15)
    * mode : loại chức năng mà bạn muốn dùng, 2 chức năng cơ bản nhất của GPIO chính là Input và Output và 1 số chức năng khác như analog ...
      * cú pháp : GPIO_Mode_IN_FLOATING /GPIO_Mode_Out_PP
      * Input (đầu vào) : có nhiều loại ví dụ như floating, input_pullup, input_pulldown
          + Input floating: Đầu vào thả nổi, điện áp không cố định dao động từ 0V tới Vcc ( thông qua điện trở )
          + Input pull up: Đầu vào có trở kéo lên (điện áp mặc định trên chân là Vcc)
          + Input pull down: Đầu vào có trở kéo xuống (điện áp mặc định trên chân là 0V)
          + GPIO_Mode_AIN: Đầu vào tương tự, dùng để đo ADC
       Để hiểu rõ hơn thì nôn na là: ví dụ bạn có 1 nút nhấn đi, bạn để trạng thái mặc định là 1(tức là chưa làm gì thì trạng thái nó là 1), khi tác động trạng thái nó là 0. 
       Lúc đó bạn thiết kế bài toán là nhấn nút led sẽ sáng thì làm sao nó biết được là bạn nhấn nút? thì lúc đó nếu nó nhận được tín hiệu 0(tức là có sự thay đổi trạng thái 
       từ 0 về 1) thì nó sẽ thực hiện lệnh bật đèn.
      * Output (đầu ra): xuất tín hiệu ra ngoài . Thường dùng 2 loại chính opendrain và push-pull(pull-up & pull-down).
          + Ouput Opendrain: Đầu ra dạng cực máng hở. Chỉ có thể kéo về 0V bằng cách ghi bit 0, khi ghi bit 1, chân IO sẽ có điện áp tương ứng với nguồn nối vào IO đó.
          Khi sử dụng mode Push-pull thì trong đó nó cấu hình sẵn một điện trở rồi(kéo lên hoặc kéo xuống) nhưng ở I2C nó đã có sẵn điện trở kéo lên rồi, nên dùng OD để      
          không cần điện trở kéo cho vi điều khiển nữa. Tại vì mình đã có điện trở kéo lên rồi giờ config thêm nó sẽ gây xung đột
          + Ouput Push Pull : tương tự như pull up và pull down của input.
          * Khi nào dùng opendrain và khi nào dùng push-pull? Khi bạn muốn on/off,pwm thì dùng push. Còn opendrain thì với những giao thức nào ở trạng thái mặc định của nó đã có trở kéo lên nguồn rồi thì trong vi điều khiển ko cần dùng nữa ví dụ:làm việc với giao thức I2C 😊
     * speed : tốc độ nhận hoặc xuất tín hiệu , có 3 loại : GPIO_Speed_10MHz ,GPIO_Speed_2MHz, GPIO_Speed_50MHz thông thường thì chọn 50MHz
* Vấn đề đặt ra ở đây là? trong một chương trình mình muốn dùng 1 lúc nhiều chân đi chẳng lẻ lần nào mình cũng gọi các tham số ra , như thế sẽ rất chi là mệt, nên để tối ưu thì mình sẽ tạo 1 struct mà nó có cùng tính chất như:mode, pin, speed. Trong thư viện chuẩn, struct đó có tên là GPIO_InitTypeDef. Struct là một kiểu dữ liệu nên mình sẽ khai báo 1 biến và dùng biến đó trỏ tới các tham số trong struct. Các cấu hình sẽ lưu giá trị vào biến đó. Vậy là đã cấu hình cho 1 chân đã xong, và mình sẽ truyền 2 tham số đó là tên GPIO:GPIO cần dùng và cấu hình mình vừa mới config: con trỏ tới biến đó.

* một số hàm thông dụng :
uint8_t GPIO_ReadInputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

uint16_t GPIO_ReadInputData(GPIO_TypeDef* GPIOx);

uint8_t GPIO_ReadOutputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

uint16_t GPIO_ReadOutputData(GPIO_TypeDef* GPIOx);

void GPIO_SetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);//0b0000….0010

void GPIO_ResetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

void GPIO_WriteBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, BitAction BitVal);

void GPIO_Write(GPIO_TypeDef* GPIOx, uint16_t PortVal);
</details>
<details>
<summary>LESSON 2: CÁC CHUẨN GIAO TIẾP </summary>
1.Truyền nhận dữ liệu :
   Việc truyền nhận dữ liệu thực chất là việc truyền nhận các tín hiệu điện áp biểu diễn cho các bit, điện áp được truyền nhận trên các chân của Vi điều khiển.
  
  ![image](https://github.com/user-attachments/assets/242557bc-9a49-408f-89aa-11dd7f3a0a53)
Câu hỏi được đặt ra ở đây là :ví dụ 2 bit liền kề nó có cùng mức điện áp thì làm sao MCU nhận biết được đó là 2 bit? => Đó là lí do tại sao có các kiểu giao tiếp sau đây.

2.SPI - Serial Peripheral Interface :
* Các bit dữ liệu được truyền nối tiếp nhau và có xung clock đồng bộ.

* Giao tiếp song công, có thể truyền và nhận cùng một thời điểm.

* Có chế độ 1 dây: truyền hoặc nhận

* Khoảng cách truyền ngắn

* Giao tiếp 1 Master với nhiều Slave.
* kết nối với nhau bằng 4 dây

![image](https://github.com/user-attachments/assets/e2144f6d-4d56-47f6-8c75-40655d24b1be)

Giới thiệu các dây:
* SCK: Serial Clock. Thiết bị tạo clock là master và cung cấp clock cho slave. Sở dỉ là giao tiếp đồng bộ vì kiểu master và slave có chung 1 dây SCK để điều khiển truyền hay nhận của 2 thiết bị.
* MOSI: Master Out Slave In . Tín hiệu tạo bởi master và Slave nhận tín hiệu.
* MISO: Master In Slave Out. Tín hiệu tạo bởi Slave và Master nhận tín hiệu
* SS: Select Slave. Chọn thiết bị Slave để giao tiếp. Để chọn được thì Master kéo đường SS tương ứng xuống mức 0(bình thường SS ở mức 1). Giao tiếp bằng chân chọn chip Quá trình truyền nhận SPI:

![image](https://github.com/user-attachments/assets/d93ba62d-ee92-4e56-bfc7-4759f91c08c5)
![image](https://github.com/user-attachments/assets/d0d1cfe5-a8fa-43ba-a513-fcf0eba2910c)

Qúa trình truyền nhận của SPi:
* Tùy thuộc vào bit chọn định dạng khung dữ liệu (DFF trong thanh ghi SPI_CR1), dữ liệu gửi hoặc nhận là 8 bit hoặc 16 bit. Lựa chọn này phải được thực hiện trước khi kích hoạt SPI để đảm bảo hoạt động chính xác.

* Bắt đầu quá trình, master sẽ kéo chân CS của slave muốn giao tiếp xuống 0 để báo hiệu muốn truyền nhận.

* Mỗi xung clock, Master sẽ gửi đi 1 bit từ thanh ghi dịch (Shift Register) của nó đến thanh ghi dịch của Slave thông qua đường MOSI. Đồng thời Slave cũng gửi lại 1 bit đến cho Master qua đường MISO.Như vậy sau 8 chu kỳ clock thì hoàn tất việc truyền và nhận 1 byte dữ liệu.
![image](https://github.com/user-attachments/assets/d4a7f903-68b0-4969-be62-7ff8f5c2e1e5)

Các mode trong SPI :
* CPOL:
   + Bằng 0 thì lúc mặc định là 0. Muốn tạo ra clock để báo hiệu truyền nhận thì ta kéo nó từ 0 lên 1 rồi về 0 là tạo ra 1 xung clock để truyền nhận 1 bit
   + Bằng 1 thì lúc mặc định là 1. Muốn tạo ra clock để báo hiệu truyềnn nhận thì ta kéo nó từ 1 xuống 0 rồi về 1 là tạo ra 1 xung clock để truyền nhận 1 bit.
*CPHA:
   + Bằng 0 :Bit dữ liệu đầu ra sẽ rơi vào cạnh xung đầu tiên của chu kì.
   + Bằng 1 :Bit dữ liệu đầu ra sẽ rơi vào cạnh xung thứ hai của chu kì.
* Tốc độ truyền khoảng vài Mbps hoặc vài chục Mbps.

![image](https://github.com/user-attachments/assets/b0e4c6b1-05f8-4360-a5e8-da50b1095606)

* Với stm32f103: tần só xung là 72MHz, qua bộ chia 8(có thể chọn bộ chia 2,4,8....) -> Tốc độ bằng 9 Mbps
* Ưu, nhược điểm của chuẩn giao tiếp SPI:

Ưu điểm:
 + Không có hệ thống định địa chỉ slave phức tạp như I2C
 + Tốc độ truyền dữ liệu cao hơn I2C (nhanh gần gấp đôi)
 + Hỗ trợ truyền dữ liệu hai chiều đồng thời (full-duplex), giúp tăng hiệu suất giao tiếp.

Nhược điểm:
 + Sử dụng bốn dây (I2C và UART sử dụng hai dây).
 + Chỉ cho phép một master duy nhất.
 + khoảng cách truyền ngắn
3.I2C - Inter-Integrated Circuit :
![image](https://github.com/user-attachments/assets/52911af4-be37-4c2d-b485-5825a446ee31)


* I2C là một giao thức truyền thông nối tiếp đồng bô, nên các bit dữ liệu truyền đi được đồng bộ hóa với xung nhịp do Master điều khiển.
* I2C có 2 dây đó là SCL và SDA vào điện trở kéo lên nguồn:
   + SDA (Serial Data) - đường truyền cho master và slave để gửi và nhận dữ liệu.
   + SCL (Serial Clock) - đường mang tín hiệu xung nhịp.
* Hoạt động ở chế độ bán song công(tức là tại một thời điểm thì nó chỉ có thể truyền hoặc nhận, nếu muốn nhận phải đợi truyền xong).
* Bán song công vì: nó chỉ có 2 dây 1 là SCL(clock) và 1 dây là SDA để truyền, chỉ có 1 dây nên không thể 1 lúc mà vừa truyền vừa nhận dữ liệu được.
* I2C nó không truyền theo từng bit giống như SPI mà nó sẽ truyền theo từng frame
* I2C giao tiếp bằng địa chỉ Data frame của I2C:
![image](https://github.com/user-attachments/assets/710a093c-52fe-4746-ba7f-c6cba82ec170)

* Qúa trình truyền dữ liệu I2C :
  + Đầu tiên phải có bit start condition( SDA kéo xuống mức 0 trước SCL để tạo ra tín hiệu, bình thường 2 dây này ở mức 1 tại vì nó được gắn vào điện trở kéo lên nguồn).
  + Tiếp theo là 7 or 10 bit địa chỉ(tùy thuộc vào chip) và 1 bit r/w.
  + Phải gửi địa chỉ vì nó cùng 1 lúc truyền nhận nhiều thiết bị, để phân biệt chúng thì mỗi con phải có 1 địa chỉ, khi nó truyền bit địa chỉ thì tất cả sẽ được nhận m, thiết bị nào ứng với địa chỉ đó thì sẽ biết là sắp có quá trình truyền/nhận. Còn bit R/W để nó nói với slave tương ứng với địa chỉ đó là nó sẽ truyền hay đọc dữ liệu(0:Read là master sẽ đọc được trạng thái của Slave, 1.Write:master viết data cho slave).
  + tiếp theo sẽ là bit ACK/NACK nếu slave nào đó gửi lại 1 bit ack điều đó có nghĩa là địa chỉ đã đúng master sẽ gửi tiếp 8 bit data, sau khi đã gửi xong 8 bit data thì bên slave lại gửi tiếp bit ACK( nếu như đã nhận đủ và thành công) hoặc bit NACK(nếu không nhận được hoặc bị lỗi khung truyền)
  + kết thúc sẽ là 1 bit stop nó sẽ kéo SDA lên mức 1 trong khi SCL vẫn ở mức 1
* lưu ý : master hoàn toàn kiểm soát SCL , slave chỉ hoạt động theo

![image](https://github.com/user-attachments/assets/77fad496-755b-498c-beb2-17035689a3cc)

3.UART - Universal Asynchronous Receiver-Transmitter
* là một giao thức truyền thông phần cứng dùng giao tiếp nối tiếp không đồng bộ và có thể cấu hình được tốc độ
* UART chỉ 2 thiết bị với nhau(giao tiếp 1 - 1)
* uart là truyền thông bất đồng bộ nên 2 chủ thể có thể biết mà tương tác với nhau thì dựa vào baudrate. Khác baudrate vẫn truyền được nhưng dư liệu truyền, nhận sẽ bị sai.
* tốc độ truyền của UART được đặt ở một số chuẩn, chẳng hạn như 9600, 19200, 38400, 57600, 115200 baud và các tốc độ khác
* UART :
   + 2 dây TX,RX
   + một master và một slave
   + Quá trình truyền dữ liệu thường sẽ diễn ra ở 3.3V hoặc 5V
* UART truyền dữ liệu nối tiếp, theo 1 trong 3 chế độ:
   * Simplex: Chỉ tiến hành giao tiếp một chiều.
   * Half duplex: Dữ liệu sẽ đi theo một hướng tại 1 thời điểm
   * Full duplex: Thực hiện giao tiếp đồng thời đến và đi từ mỗi master và slave
* Data frame UART :
![image](https://github.com/user-attachments/assets/1921d7c6-365c-400d-9a6e-e86cc09c2a26)
* Để bắt đầu truyền nhận khi có Start Bit(kéo TX từ 1->0).
* có 5 đến 9 bit dữ liệu.
* 0 to 1 Parity bit (bit chẵn lẻ).
   * Bit chẵn lẻ kiểm tra xem dữ liệu nhận đúng hay chưa.
   * Quy luật chẵn: nếu tổng số bit 1 là số chẵn thì bit đó là 0, còn quy luật lẻ là nếu tổng sốt bit lẻ là số chẵn thì thêm số 1.
* 1 hoặc 2 stop bit(kéo chân Rx lên 1).
</details>
<details>
  <summary> CONFIG PERIPHERAL</summary>
  * Các giao thức trong chúng ta đều có thể triển khai bằng SOFTWARE và HARDWARE :
  
   + SOFTWARE : thường dùng để mô phỏng lại quá trình giao thức đó hoạt động mà không cần phải sử dụng đến chân có sẵn trong mạch mà vẫn có thể biến các chân GPIO thành nó hay còn gọi là giả lập giao thức (em có để mô phỏng ở trên)
    
   + HARDWARE : dùng các chân được tích hợp trong mạch giúp dễ dàng triển khai các chức năng của giao thức
  * datasheet :

    ![image](https://github.com/user-attachments/assets/2f4bdc01-1db4-4740-8d18-1290caf3221a)

1.SPI - HARDWARE :
  * STM32F1 có 2 khối SPI, SPI1 ở APB2 và SPI2 ở PAB1. Các khối này được xây dựng các kết nối, driver và các hàm riêng trong bộ thư viện chuẩn.
  b1. đầu tiên phải cấu hình chân SPI  bằng cách đọc datasheet :

    vd : tôi chọn SPI1 thì cần cấu hình chân này :
    
    ![image](https://github.com/user-attachments/assets/1cd6a87e-6546-4b3b-ad23-059c1cce3af7)
    
  b2. cấu hình tham số của SPI :

  ![image](https://github.com/user-attachments/assets/a17aac75-2ff3-498a-8cf7-a7f751b2ae0f)

  b3. tùy hình vào cấu hình truyền hoặc nhận ta sẽ có các hàm khác nhau :
  
  ![image](https://github.com/user-attachments/assets/c0488c58-6527-40ca-85bb-97e7bfdbc230)
  
* Một số hàm và cờ quan trọng :
![image](https://github.com/user-attachments/assets/f585370b-cb97-4191-8fc7-e982ac4a3744)

2. I2C - HARDWARE :
 * tương tự như SPI , Các bước thực hiện là: Xác định các chân I2C -> Cấu hình GPIO -> Cấu hình I2C

![image](https://github.com/user-attachments/assets/407157d0-59d1-4ff2-8bce-67d50b72ef6f)
+ cấu hình I2C :

![image](https://github.com/user-attachments/assets/3b61633d-1859-46d4-ae49-0faa0ccec37f)

* cấu hình truyền hoặc nhận :

  ![image](https://github.com/user-attachments/assets/a886b2c3-935a-4415-bd44-2b28c43e699e)

  ![image](https://github.com/user-attachments/assets/fa358374-99bb-4c8b-8fbc-8c338adcdd93)

* một số hàm và cờ quan trọng :

![image](https://github.com/user-attachments/assets/d7ce6cd0-0bbe-4210-a55a-209f1b052c63)

![image](https://github.com/user-attachments/assets/7d2053b2-622b-491c-8532-67155d32bdc9)

3. UART - HARDWARE :
* Các bước cấu hình uart : xác định chân UART -> cấu hình chân GPIO -> Cấu hình UART
 + cấu hình chân GPIO :

![image](https://github.com/user-attachments/assets/de2770b7-87f9-4b6d-b840-5e184e14c70b)

![image](https://github.com/user-attachments/assets/ce77d80c-9e50-43a6-9a3c-ff4dce77764c)
 + cấu hình uart :

![image](https://github.com/user-attachments/assets/70ca7997-7653-42d6-8857-b385e260ffeb)

+ hàm truyền hoặc nhận :

  ![image](https://github.com/user-attachments/assets/fbc63be8-3363-4eb9-84bc-e8e481046e48)
+ một số cờ quan trọng :

![image](https://github.com/user-attachments/assets/22253543-2810-4578-acc6-7e216f89fb2e)
</details>
<details>
  <summary>LESSON 3: Ngắt ngoài và ngắt truyền thông</summary>
* Ngắt là 1 sự kiện khẩn cấp xảy ra trong hay ngoài vi điều khiển. Nó yêu MCU phải dừng chương trình chính và thực thi chương trình ngắt. Hàm phục vụ ngắt.
  
* Sau khi xử lí xong nhiệm vụ này thì bộ đếm chương trình sẽ trả về giá trị trước đó để bộ xử lí thực hiện công việc còn đang dang dở. Như vậy, ngắt có mức độ ưu tiên cao nhất, thường xử lí các sự kiện bất ngờ nhưng không tốn thời gian. Ngắt có 2 loại: ngắt có thể xuất phát từ tín hiệu bên trong con chip(ngắt báo bộ đếm timer/counter tràn, ngắt báo quá trình gửi dữ liệu bằng RS232 kết) hoặc ngắt do các tác nhân bên ngoài(nhấn button, ngắt báo có 1 gói dữ liệu nhận được).

VD : Cách chương trình chạy ngắt thực hiện :

![image](https://github.com/user-attachments/assets/4b6982b5-09ed-4115-bcc8-01f7663c685f)

1.Ngắt ngoài :
Ngắt ngoài là 1 sự kiện ngắt xảy ra khi có tín hiệu can thiệp từ bên ngoài, từ phần cứng, người sử dụng hay ngoại vi...
 * Ngắt ngoài của chip STM32F103 bao gồm có 16 line:

![image](https://github.com/user-attachments/assets/a233d50d-402f-4d9b-a20c-fa07f0136527)
  + mỗi line chỉ được gắn với một một port duy nhất và không thể cấu hình cho các port khác , vì thế chúng ta cần phải phân chia các line 1 cách phù hợp để không bị trùng ngắt.

     vd : Line0 nếu chúng ta đã chọn chân PA0 (chân 0 ở port A) làm chân ngắt thì tất cả các chân 0 ở các Port khác không được khai báo làm chân ngắt ngoài nữa
    
  + các Line ngắt sẽ được phân vào các Vector ngắt tương ứng. Các Line ngắt của chip STM32F103 được phân bố vào các vector ngắt như sau:
    
    ![image](https://github.com/user-attachments/assets/bdca1584-d56a-4a1a-b6cb-1ae6779c7ec7)
    
    + Các Line0, Line1, Line2, Line3, Line4 sẽ được phân vào các vector ngắt riêng biệt EXTI0, EXTI1, EXTI2, EXTI3, EXTI4, còn từ Line5->Line9 sẽ được phân vào vector ngắt EXTI9_5, Line10->Line15 được phân vào vecotr EXTI15_10.

    + các ngắt quyết định mức độ thực hiện thông qua mức độ ưu tiên :
       + ngắt nào có độ ưu tiên cao hơn thì ngắt đó thực hiên trước.
       + MCU sẽ kiểm tra ngắt có cùng một mức Preemption Priority thì ngắt nào có Sub Priority cao hơn thì ngắt đó được thực hiện trước.
       + Còn trường hợp 2 ngắt có cùng mức Preemption và Sub Priority luôn thì ngắt nào đến trước được thực hiện trước.
      
* Để sử dụng ngắt ngoài ta thực hiện các bước sau: Xác định các chân ngắt ngoài -> Cấu hình các chân GPIO
   + Trước tiên muốn sử dung bất cứ ngoại vi nào thì mình phải bật Clock của bus gắn với ngoại vi đó, ngoài ra phải bật thêm AFIO. AFTO là những cái funtion thay thế. 
      + cấu hình RCC :
        
     ![image](https://github.com/user-attachments/assets/980a360c-bc39-4a22-80bf-24ebd0a3f0f8)

      + sau đó cấu hình GPIO:
     
     ![image](https://github.com/user-attachments/assets/54033f18-9f32-472a-9115-a91c1c95bf22)

      + Cấu hình NVIC :
        trước hết chúng ta cần phải tra bảng NVIC:
        
        ![image](https://github.com/user-attachments/assets/de5d5a22-d9d6-41cf-adab-77c3292b3755)
        
        Sau đó cấu hình NVIC tùy theo Group và mức độ ưu tiên khi tra :
        
        ![image](https://github.com/user-attachments/assets/82e0fdca-fe35-4caa-bba3-9e88cf93e11f)

        vd : ![image](https://github.com/user-attachments/assets/6ca07c33-f3a8-4b02-af8e-80402349cf6a)

     + cấu hình EXTI :
       
       ![image](https://github.com/user-attachments/assets/8aa8a1a4-76a3-4a55-ae61-934d721f832d)

       VD : ![image](https://github.com/user-attachments/assets/61853b7b-49a8-44bd-b28b-454a5ae6ec34)

     + sau cùng là hàm phục vụ ngắt ngoài : EXTIx_IRQHandler() (x là line ngắt tương ứng), Hàm EXTI_GetITStatus(EXTI_Linex) :Kiểm tra cờ ngắt của line x tương ứng. Hàm EXTI_ClearITPendingBit(EXTI_Linex): Xóa cờ ngắt ở line x.

     ![image](https://github.com/user-attachments/assets/ce2e8acb-0135-4baa-ae8e-3c9a03d1ce4f)
      
2. Ngắt truyền thông :
* STM32F103 hỗ trợ các ngắt cho các giao thức như : SPI, I2C, UART ... các ngắt này giúp cho việc truyền hoặc nhận dữ liệu không bị sai sót và tránh bị mất dữ liệu
* ở đây em sẽ vd về UART với ngắt nhận , các giao thức còn lại cũng tương tự như vậy :
  
  các bước cấu hình ngắt uart như sau : cấu hình chân GPIO -> cấu hình NVIC -> Cấu hình uart.
  
  + bật clock :
    
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_USART1,ENABLE);
  + cấu hình GPIO :
    
    ![image](https://github.com/user-attachments/assets/94e434ed-8f1a-47df-9671-b1bb9310f8e2)
    
  + cấu hình NVIC :
    
    ![image](https://github.com/user-attachments/assets/6a2628fc-b85c-4d77-9b1c-dfeeef49bad5)
    
  + cấu hình uart :

    ![image](https://github.com/user-attachments/assets/c43d8978-4306-4d08-b3e1-a9792a7d7b8f)

  + hàm ngắt uart :
    
    ![image](https://github.com/user-attachments/assets/d1c95439-f349-4623-ac24-0cdb679049a4)
 
    + Trong hàm phục vụ ngắt, ta kiểm tra ngắt đến là ngắt nhận RXNE hay ngắt truyền TX, bằng  hàm USART_GetITStatus. Tùy theo tín hiệu ngắt mà có thể lập trình tác vụ khác nhau. Sau khi thực thi xong, có thể xóa cờ ngắt để đảm bảo không còn ngắt trên line (thông thường cờ ngắt sẽ tự động xóa).
</details>
<details>
  <summary>LESSON 4: ADC </summary>
* Vi điều khiển hay các thiết bị ngày nay đều sử dụng tín hiệu số dựa trên các bit nhị phân để hoạt động. Còn thực tế thì không chỉ mãi là tín hiệu số mà là tín hiệu tương tự và liên tục vì vậy cần phải có thiết bị chuyển đổi từ tín hiệu tương tự sang tín hiệu số. *ADC - Analog to Digital Convert*: bộ chuyển đổi từ tín hiệu tương tự sang tín hiệu số

![image](https://github.com/user-attachments/assets/6369b5d8-2b97-4c49-b384-ceabcd2db3d3)

Khả năng chuyển đổi của ADC phụ thuộc vào 2 yếu tố
 + Độ phân giải: Số bit mà ADC sử dụng để mã hóa tín hiệu. Hay còn gọi là số mức tín hiệu được biểu diễn(có độ phân giải càng cao thì độ chính xác càng lớn.)
 + Tần số/Chu kì lấy mẫu: tốc độ/khoảng thời gian giữa 2 lần mã hóa(tần số lấy mẫu càng cao thì chuyển đổi sẽ có độ chính xác càng lớn). Tần số lấy mẫu = 1/(thời gian lấy mẫu + thời gian chuyển đổi).
Trong stm32 có 2 kênh ADC đó là ADC1 và ADC2, mỗi bộ có tối đa 9 channel với nhiều mode hoạt động, kết quả chuyển đổi được lưu trong thanh ghi 16 bit.
 + Độ phân giải: 12 bit
 + Có các ngắt hổ trợ, có thể điều khiển hoạt động ADC bằng xung Trigger.
 + Thời gian chuyển đổi nhanh: 1us tại tần số 65Mhz.
 + Có bộ DMA giúp tăng tốc độ xử lí Sơ đồ khối bộ ADC:

![image](https://github.com/user-attachments/assets/a4ca6023-cb18-47e8-8baa-7a69def828c0)

Các bước thực hiện ADC :

Cấu hình GPIO -> Cấu hình ADC

 + xác định chân và port :
![image](https://github.com/user-attachments/assets/325f6f30-0f71-43e2-9779-b8edb0bc2a6a)

 + Các chế độ của ADC:
   + Single: ADC chỉ đọc 1 kênh duy nhất, và chỉ đọc khi kênh nào được yêu câu.
   + Single Continous: sẽ đọc 1 kênh duy nhất, nhưng đọc dữ liệu nhiều lần
   + Scan: Multi-Channels: Quét qua và đọc dữ liệu nhiều kênh, nhưng chỉ đọc khi nào được yêu cầu.
   + Scan: Continous Multi-Channels Repeat: Quét qua và đọc dữ liệu nhiều kênh, nhưng đọc liên tiếp nhiều lần giống như Single Continous.

![image](https://github.com/user-attachments/assets/e6174bc2-da3b-42f0-a1a2-dbcba43ba2ea)

</details>
