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
  + Tiếp theo là 7 or 10 bit địa chỉ(tùy thuộc vào chip) và 1 bit w/r.
  + Phải gửi địa chỉ vì nó cùng 1 lúc truyền nhận nhiều thiết bị, để phân biệt chúng thì mỗi con phải có 1 địa chỉ, khi nó truyền bit địa chỉ thì tất cả sẽ được nhận m, thiết bị nào ứng với địa chỉ đó thì sẽ biết là sắp có quá trình truyền/nhận. Còn bit R/W để nó nói với slave tương ứng với địa chỉ đó là nó sẽ truyền hay đọc dữ liệu(0:Read là master sẽ đọc được trạng thái của Slave, 1.Write:master viết data cho slave).
  + tiếp theo sẽ là bit ACK/NACK nếu slave nào đó gửi lại 1 bit ack điều đó có nghĩa là địa chỉ đã đúng master sẽ gửi tiếp 8 bit data, sau khi đã gửi xong 8 bit data thì bên slave lại gửi tiếp bit ACK( nếu như đã nhận đủ và thành công) hoặc bit NACK(nếu không nhận được hoặc bị lỗi khung truyền)
  + kết thúc sẽ là 1 bit stop nó sẽ kéo SDA lên mức 1 trong khi SCL vẫn ở mức 1
* lưu ý : master hoàn toàn kiểm soát SCL , slave chỉ hoạt động theo

![image](https://github.com/user-attachments/assets/77fad496-755b-498c-beb2-17035689a3cc)

3.UART - Universal Asynchronous Receiver-Transmitter
</details>
