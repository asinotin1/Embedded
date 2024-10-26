![image](https://github.com/user-attachments/assets/dcc4ec9b-4188-4901-be5a-22e2e99c6bbb)<details>
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
<details>
  <summary>LESSON 5: DMA </summary>
  * DMA – Direct memory access được sử dụng với mục đích truyền data với tốc độ cao từ thiết bị ngoại vi đến bộ nhớ cũng như từ bộ nhớ đến bộ nhớ
  * Với DMA, dữ liệu sẽ được truyền đi nhanh chóng mà không cần đến bất kỳ sự tác động nào của CPU. Điều này sẽ giữ cho tài nguyên của CPU được rảnh rỗi cho các thao tác khác. Đồng thời tránh việc data nhận về từ ngoại vi bị mất mát
  
  ![image](https://github.com/user-attachments/assets/6486c7a3-a68c-4935-91d3-df3c1b6564c2)

  * DMA có thể điều khiển data truyền từ :
    + Bộ nhớ đến Peripheral
    + Ngược lại, Periph đến Bộ nhớ.
    + Giữa 2 vùng nhớ.
  * Sơ đồ khối bộ DMA :

    ![image](https://github.com/user-attachments/assets/4efaf9d9-aef4-4c68-a1a3-b47772a78db6)
  * STM32F1 có 2 bộ DMA với nhiều kênh, mỗi kênh có nhiều ngoại vi có thể dùng DMA như bảng:

    ![image](https://github.com/user-attachments/assets/df2194b2-25f5-4328-91e9-6b7691982600)
    
    ●	Các Channel đều có thể được cấu hình riêng biệt.
    
    ●	Mỗi Channel được kết nối để dành riêng cho tín hiệu DMA từ các thiết bị ngoại vi hoặc tín hiệu từ bên trong MCU.
    
    ●	Có 4 mức ưu tiên có thể lập trình cho mỗi Channel.
    
    ●	Kích thước data được sử dụng là 1 Byte, 2 Byte (Half Word) hoặc 4byte (Word)
    
    ●	Hỗ trợ việc lặp lại liên tục Data.
    
    ●	5 cờ báo ngắt (DMA Half Transfer, DMA Transfer complete, DMA Transfer Error, DMA FIFO Error, Direct Mode Error).
    
    ●	Quyền truy cập tới Flash, SRAM, APB1, APB2, APB.
    
    ●	Số lượng data có thể lập trình được lên tới 65535.
    
    ●	Đối với DMA2, mỗi luồng đều hỗ trợ để chuyển dữ liệu từ bộ nhớ đến bộ nhớ
 * DMA có 2 chế độ hoạt động :
   + Normal mode: Với chế độ này, DMA truyền dữ liệu cho tới khi truyền đủ 1 lượng dữ liệu giới hạn đã khai báo DMA sẽ dừng hoạt động. Muốn nó tiếp tục hoạt động thì phải khởi động lại
   
   + Circular mode: Với chế độ này, Khi DMA truyền đủ 1 lượng dữ liệu giới hạn đã khai báo thì nó sẽ truyền tiếp về vị trí ban đầu (Cơ chế như Ring buffer).
* cấu hình DMA :
 + Không như các ngoại vi khác, DMA cần được cấp xung từ AHB, cả 2 bộ DMA đều có xung cấp từ AHB. Ngoài ra cần cấp xung cho AFIO.
   
 ![image](https://github.com/user-attachments/assets/2e80e0a9-833c-4df5-96c0-ef17df808435)

+ Sau khi cấu hình cho DMA xong, chỉ cần gọi hàm DMA_Cmd cho ngoại vi tương ứng. Bộ DMA sẽ tự động truyền nhận data cũng như ghi dữ liệu vào vùng nhớ cụ thể. 
</details>
<details>
  <summary>LESSON 6: Timer & PWM </summary>
  + timer là 1 mạch digital logic có vai trò đếm mỗi chu kỳ clock (đếm lên hoặc đếm xuống).
  
  + Timer còn có thể hoạt động ở chế độ counter, nó sẽ nhận xung clock từ các tín hiệu ngoài. Có thể là từ 1 nút nhấn, bộ đếm sẽ được tăng sau mỗi lần bấm nút (sườn lên hoặc sườn xuống tùy vào cấu hình).
  
  + STM32f103C8 có tất cả 7 timer nhưng trong đó đã bao gồm 1 systick timer, 2 watchdog timer. Vậy chỉ còn lại 4 timer dùng cho các chức năng như ngắt, timer base, PWM, Encoder, Input capture
  
  + TIM1 là Timer đặc biệt, chuyên dụng cho việc xuất xung với các mode xuất xung, các mode bảo vệ đầy đủ hơn so với các timer khác. TIM1 thuộc khối clock APB2
  
  + TIM2,TIM3,TIM4 thuộc nhóm APB1.
    
  + ![image](https://github.com/user-attachments/assets/8dec8c19-2755-43e0-b802-4448119d8b21)

    + Khi không có cấu hình gì liên quan đến clock và đã gắn đúng thạch anh ngoài trên chân PD0(5) và PD1(6) thì clock tương ứng của TIM1,TIM2,TIM3,TIM4 đã là 72Mhz. Cần ghi nhớ là sử dụng timer nào thì cấp clock cho timer đó theo đúng nhánh clock.
   
    + Prescaler là bộ chia tần số của timer, giá trị tối đa là 65535. Các giá trị này có thể được thay đổi và điều chỉnh bằng lập trình
   
    + Auto Reload value là giá trị bộ đếm tối đa có thể được điều chỉnh để nạp vào cho timer. Giá trị bộ đếm này được cài đặt tối đa là 16bit tương ứng với giá trị là 65535.

    + FTIMER= fSYSTEM/[(PSC+1)(Period+1)]

    ![image](https://github.com/user-attachments/assets/c10a0585-88e9-4ccc-aa89-55befe304c1a)

    cấu hình timer :
    
    ![image](https://github.com/user-attachments/assets/4f7afb24-fa55-4c7b-9c71-9c346e97c52b)

    7199 tương ứng với giá trị PSC, 9999 tương ứng với Period. Clock cung cấp cho TIM4 là 72Mhz. Tính theo công thức ta sẽ được thời gian ngắt tràn là 1s.
    
   + PWM - Pulse Width Modulation là phương pháp điều chỉnh độ rộng của xung có chu kì cố định, nhằm tạo ra sự thay đổi điện áp tại đầu ra.
     
   + PWM ứng dụng nhiều trong việc điều khiển động cơ, các bộ nguồn xung boot, buck, nghịch lưu 1 pha, 3 pha…

   + Trong mỗi Timer có 4 kênh độc lập phát PWM

   + Chu kì xung của PWM được quản lý bằng thanh ghi PSC và thanh ghi ARR.

   +  Duty Cycles được quản lý bằng thanh ghi CCR

   +  Tín hiệu PWM có hai yếu tố quan trọng:
     
      + Tần số: Là số lần tín hiệu lặp lại trong một giây. Đối với servo, tần số thông thường là 50Hz (tức là, chu kỳ lặp lại sau mỗi 20ms).

      + Độ rộng xung (Pulse Width ): Là thời gian tín hiệu ở mức cao trong mỗi chu kỳ. Độ rộng xung thường được đo bằng microsecond (µs) và quyết định góc mà servo sẽ xoay đến.Tỉ lệ độ rộng xung với chu kì xung gọi là chu kì nhiệm vụ(Duty Cycle).

  ![image](https://github.com/user-attachments/assets/79e870ae-9320-4df1-aecd-bac0ba9fa909)

  + cấu hình PWM : chúng ta cần phải xác định các chân của chip có pwm thông qua datasheet
    
    vd : stm32f103c8t6
    
    ![image](https://github.com/user-attachments/assets/88c0e9cd-dffd-4daa-a15f-784655d5a76b)

  + Timer có hỗ trợ chế độ PWM, ngõ ra của xung sẽ được xuất trên các chân GPIO tương ứng với từng kênh của Timer, Mode thường dùng là AF_PP.
Phải cấu hình cho các chân này, đồng thời bật RCC cho AFIO.

![image](https://github.com/user-attachments/assets/3f4d1b0d-ef7e-4d77-bc32-d0eb15ae5f2d)

![image](https://github.com/user-attachments/assets/0ee997f4-07aa-4bb8-849a-d114ad4513ba)

![image](https://github.com/user-attachments/assets/d17f643e-6fd1-4c17-9dfc-94e371c6cd1d)
       
</details>
<details>
  <summary> LESSON 7: Flash & BootLoader</summary>
  Nguồn tham khảo: Lập trình điện tử Chắc hẳn các bạn đã từng nghe quá trình boot trong máy tính, nó là một phần quan trọng giúp cho việc khởi động hệ điều hành máy tính. Bootloader là phần mềm quan trọng nạp vào máy tính trước khi khởi động. Ta học vi điều khiển phải cần biến đến Bootloader 👉Rỏ ràng máy tính cũng xử dụng nhiều vi xử lí, Bootloader như một firmware nạp sẵn, trước khi máy tính khởi động thì cái firmware này sẽ được gọi, Vi điểu khiển của chúng ta cũng sử dụng lõi vi xử và khi khởi động thì nó sẽ bắt đầu từ đâu?

![image](https://github.com/user-attachments/assets/9db7364e-40ed-4bb6-9fdc-15efe807e1af)

Nếu có một chương trình bootloader nạp vào trong vi điều khiển, thì trước tiên nó nhảy vào chương trình bootloader thực hiện một số công việc, sau đó mới thực hiện Application.

Tổ chức bộ nhớ STM32

 * Hiểu được tổ chức bộ nhớ STM32 rất quan trọng trong bootloader. Chúng ta cần nắm chương trình boot nằm ở địa chỉ nào, chương trình nằm ở địa chỉ nào và cách phân chia Page của bộ nhớ FLash(stm32f1). Trong vi điều khiển STM có 2 bộ nhớ cần được quan tâm là: bộ nhớ chương trình và bộ nhớ dữ liệu:
   
   + Vùng nhớ code: có thể là FLASH/EEPROM/ROM/OTP,... dùng để lưu code và các lệnh của chương trình
     
   + Vùng nhớ SRAM: sử dụng để kế nối Sram trên chip, dùng để lưu dữ liệu tạm thời khi run-time.

   ![image](https://github.com/user-attachments/assets/65273a4a-0ff3-42e6-bbd7-6ce2ad519be5)

  * Địa chỉ bộ nhớ Flash bắt đầu 0x00000000 nhưng trong vi điều khiển STM32, vùng nhớ code bắt đầu từ địa chỉ 0x0800 0000 khi mình nạp xuống thì nó sẽ mặc định nạp chương trình từ địa chỉ này, với MSP ở 0x0800 0000 và Vector Table bắt đầu từ địa chỉ 0x0800 0004 (Reset_Handler).

![image](https://github.com/user-attachments/assets/5fa1258f-8f66-48fc-86aa-9207916457c6)

Vi điều khiển STM32F1 cung cấp 128/64Kb, ngoài lưu trữ MSP, Vector Table, bộ nhớ Flash sẽ lưu trữ vùng nhớ chương trình ứng dụng của chúng ta, cùng với đó là vùng data.

👉Để thao tác với bộ nhớ hiệu quả thì bộ nhớ Flash trong STM32 chia thành các Page, mỗi Page có kích thước 1Kb.

👉Bộ nhớ Flash có thể được thao tác ghi trên từng word(2bytes/4bytes) nhưng lại chỉ có thể xóa theo từng Page =>Vì vậy , chúng ta có thể thực hiện Bootloader bằng cách cài đặt chương trình Bootloader ở một Page nào đó, chẳng hạn như Page0, Và cùng lúc đó đặt Firmware application 1 vào Page1, Firmware application 2 vào Page2, Firmware application 3 vào Page3.

![image](https://github.com/user-attachments/assets/6a3b585c-ad43-4831-87a0-cb54dbbd7685)

Chúng ta sẽ bắt đầu với chương trình Bootloader, được đặt tại địa chỉ 0x0800 0000. Ngoài ra, chúng ta thấy sự xuất hiện của 3 Firmware khác:

 + Factory Firmare: là phiên bản đầu tiên của Firmware mà nhà sản xuất cung cấp cho người dùng.

 + Current Firmware: là phiên bản hiện tại của Firmware đang chạy trên vi điều khiển, được chúng ta lưu trên 1 Page nào đó.

 + FOTA Firmware: là phiên bản cập nhật mới của firmware.

File nhị phân :

Có một số định dạng file nhị phân thường gặp là: .BIN, .ELF, .HEX

![image](https://github.com/user-attachments/assets/a076bb80-5dfe-43bc-85cb-4185958f44a1)

![image](https://github.com/user-attachments/assets/d2a26374-af43-4aaf-b240-e20e5d8e0c79)

![image](https://github.com/user-attachments/assets/186ce96f-db6e-4da6-a051-578baf62aaf5)

![image](https://github.com/user-attachments/assets/34014057-c1fe-4846-b692-96c247638e18)

* Data: Phần này là dữ liệu sẽ lưu lên FLASH, số byte sẽ được quy định ở trường Byte Count. Checksum: gồm 2 chữ số, dùng để kiểm tra lỗi. Theo quy định thì một line sẽ đúng khi mà byte checksum sẽ bằng đảo của tổng tất cả các byte còn lại cùng dòng, cộng thêm 1.

Có 3 loại thao tác với bộ nhớ Flash: đọc - Read, ghi - Write, xóa - Delete.

</details>
