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


* Tốc độ truyền khoảng vài Mbps hoặc vài chục Mbps.

![image](https://github.com/user-attachments/assets/b0e4c6b1-05f8-4360-a5e8-da50b1095606)

* Với stm32f103: tần só xung là 72MHz, qua bộ chia 8(có thể chọn bộ chia 2,4,8....) -> Tốc độ bằng 9 Mbps
* 


    
</details>
