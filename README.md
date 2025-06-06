# 🐔 Crossy Road - 2D edition
Crossy Road được biết đến là một tựa game mobile và với phiên bản 2D này thì tựa game được làm dựa trên các thư viện hỗ trợ đồ họa, âm thành, hình ảnh: 

- 🎮 [SDL2](https://www.libsdl.org/)
- 🖼️ [SDL_image](https://www.libsdl.org/projects/SDL_image/)
- 🔊 [SDL_mixer](https://www.libsdl.org/projects/SDL_mixer/)
- 🔤 [SDL_ttf](https://www.libsdl.org/projects/SDL_ttf/)

# 🎯 Các thao thác với Game
## 0. 💻 Hướng dẫn cài Game
- **CÁCH 1** 📥: 
    * Chọn Code -> Download ZIP

![Image](https://github.com/user-attachments/assets/a1e76528-1281-4616-87e6-9c5cfa3e6ba2)

* Sau khi tải xong file ZIP thì giải nén và bật CrossyRoad.exe để chơi

- **CÁCH 2** 🛠️: 
    * Clone repo này về 

    ```sh
    git clone https://github.com/NguyenHaiDang24022625/headache_issue.git
    ```
    * Cài mingw phiên bản MinGW-w64 [tại đây](https://www.mingw-w64.org/downloads/)

    * Sau khi cài thêm đường dẫn bin vào biến mỗi trường PATH như sau:

        + vào công cụ tìm kiếm trên máy tính gõ tìm Edit the system environment variables.

        + chọn Environment Variables -> chọn PATH -> chọn Edit -> chọn New và thêm đường dẫn bin -> chọn OK để hoàn tất.

    * Nhớ xóa ba dll của c++ trong thư mục chứa pvz.exe: libgcc_s_seh-1.dll, libstdc++-6.dll, libwinpthread-1.dll. Ba dll 

    này chỉ dành cho người chơi không cài sẵn c++ trong máy.

    * Vào terminal rồi cd đến folder headache_issue và gõ 

    ```sh
    mingw32-make
    ```
    * Cuối cùng nhập
    ```sh
    ./CrossyRoad.exe
    ```
    là xong.

## 1. 🖥️ Màn hình chính:
Khi bắt đầu Game, màn hình sẽ hiển thị 3 nút là Play, LeaderBoard và mute.

![Image](https://github.com/user-attachments/assets/1d83ed4c-66c5-4726-aece-c2fb7f2759f9)

Một khi đã mở Game thì có thể thoát bằng phím Esc hoặc dấu "x" ở góc phải màn hình

![Image](https://github.com/user-attachments/assets/0f8d4d8d-d9fb-406c-aecd-ec83f9b4fe63)

## 2. 🏆 Màn hình LeaderBoard
Khi bấm vào nút leaderboard, bảng xếp hạng sẽ hiện ra

![Image](https://github.com/user-attachments/assets/a6d592b1-ed36-4c69-853c-c42225fa73da)

## 3. 🔇 Nút mute 
Khi bấm vào nút mute thì âm thành sẽ được tắt

![Image](https://github.com/user-attachments/assets/b3393683-e15d-40eb-8bc1-4ce846c45952)

## 4. ▶️ Nút Play 
Khi bấm vào nút Play thì logo của Game sẽ chi chuyển ra ngoài khung hình và người chơi bắt đầu chơi

![Image](https://github.com/user-attachments/assets/0d299fc2-155e-4d16-9a3e-c20fb3425356)

## 5. ⏸️ Nút Pause
Trong khoảng thời gian chơi thì sẽ có nút Pause ![Image](https://github.com/user-attachments/assets/a30a35d4-80fb-4f1a-9ea1-8cab260590f4) để dừng Game

![Image](https://github.com/user-attachments/assets/b48ed263-9f4b-4c72-89ed-17de1cf7200d)

## 6. ✏️ Ghi tên người chơi 
Khi người chơi va chạm với vật cản hay môi trường có ghi nhận va chạm thì Game sẽ cho người chơi điền tên để lên bảng xếp hạng nếu muốn

![Image](https://github.com/user-attachments/assets/cc44b296-0e62-4111-8ead-30a96ebe8a15)

## 7. 🔢 Hiển thị điểm số hiện tại và highscore
Ngoài ra khi chơi, Game sẽ hiển thị số điểm hiện tại và highscore, mỗi một ô tiến thì sẽ có điểm

![Image](https://github.com/user-attachments/assets/52e5a777-70de-41c3-9424-d2eef2dc4adc)

# 🕹️ Các thành phần trong Game
## 1. 🚗 Phương tiện trên đường  
Những phương tiện tham gia giao thông đi qua lại trên đường thao các chu kì khác nhau nếu người chơi va chạm thì sẽ phải chơi lại từ đầu

![Image](https://github.com/user-attachments/assets/5635171d-dd34-4d93-94ba-463a589af3ef)

## 2. 💧 Nước 
Khi người chơi va chạm với nước thì sẽ phải chơi lại từ đầu

![Image](https://github.com/user-attachments/assets/db491b31-e5b4-4db4-9bc8-4de316dfaa47)

## 3. 🚧 Vật cản 
Vật cản sẽ cản người chơi lại, không cho di chuyển qua

![Image](https://github.com/user-attachments/assets/723c0276-b983-434d-bae1-0152ccaed895)![Image](https://github.com/user-attachments/assets/66652c92-fbaf-418f-ac1c-9a6a515f3fb1)![Image](https://github.com/user-attachments/assets/de8e9082-5836-485d-9fd0-1cc9dbffc394)

## 4. 🏁 Đích
Khi người chơi đến đích thì sẽ được ghi tên để lên bảng xếp hạng và bắt đầu một lượt chơi mới

![Image](https://github.com/user-attachments/assets/9cccd610-6604-426e-a2ad-7dac390b371f)

# 🎮 Cách chơi 
+ 🎮 Người chơi sẽ sử dụng phím **A,W,S,D** để di chuyển nhân vật và phím **SHIFT** để tăng tốc cho nhân vật (và nhớ tắt UNIKEY để có trải nghiệm tốt nhất).
+ 🏃 Trong lúc di chuyển người chơi cố gắng di chuyển lên trên để về đích cũng như tránh các chướng ngại vật như các phương tiện giao thông, nước.
+ 💥 Khi Game ghi nhận va chạm của người chơi với chướng ngại vật thì sẽ hiện lên điểm và cho phép người chơi nhập tên của mình. Người chơi nhấn phím **ENTER** để hoàn thành việc nhập tên và hệ thống sẽ ghi nhận điểm và tên của người chơi vào LeaderBoard nếu người chơi nắm trong top 10 người có điểm số cao nhất.

# 📁 Game source code
- **📂 assets**: là folder của tư liệu dùng đến trong Game
    * **🔤 fonts**: folder chứa phông chữ.
    * **🖼️ images**: folder chứa hình ảnh.
    * **🗺️ maptile**: folder chứa các ô ghép lại để tại ra map, mỗi ô có kích thước 32 x 32 pixels, ngoài ra còn có file txt để lưu các loại phương tiện và vị trí, vận tốc của mỗi phườn tiện.
    * **📊 score**: folder chứa hai file dạng text để lưu LeaderBoard và điểm highscore.
    * **🔊 sound**: folder chứa âm thanh sử dụng trong Game: âm thanh gà bị đâm, âm thanh gà di chuyển, âm thanh khi chuột bấm xuống, âm thanh khi chuột bấm lên, nhạc nền cho Game.
- **⚙️ ECS**: là folder chứa hệ thống thành phần thực thể, hiểu một cách dơn giản trong Game sẽ có những thành phần như người chơi, phương tiện giao thông, hay là các nút có thể thao tác trong trò chơi là các thực thể (entities). Các thực thể sẽ có những thành phần (components) khác nhau như thành phần xác định sự thay đổi của vị trí (TransformComponent), thành phần hiển thị hình ảnh (SpriteComponent), ... và chúng được liên kết với nhau thành hệ thống (system).
    * **🎬 Animation.h**: file header chịu trách nhiệm chuyển đổi các hình ảnh trong từng khung hình.
    * **💢 ColliderComponent.h**: file header sử lí các thành phần có thể xảy ra va chạm (collider) như người chơi, phương tiện giao thông hay các vật cản.
    * **📋 Components.h**: file header include tất cả các Components để khi dùng tới các Components ở file khác thì chỉ cần include file này.
    * **🧩 ECS.h**: File header chứa ba lớp chính:
        + **🧩 Entity**: Quản lý các thao tác của entity như thêm/xóa Component, kiểm tra entity có Component không, truy xuất Component.
        + **🧩 Manager**: Quản lý vòng đời của các entities (thêm, xóa, nhóm entities theo chức năng).
        + **🧩 Component**: Lớp cha abstract của các components. Sử dụng virtual functions để áp dụng tính đa hình, cho phép các lớp con override các phương thức cần thiết.
    * **⌨️ KeyboardController.h**: Xử lý input từ bàn phím để điều khiển nhân vật (di chuyển, nhập tên).
    * **💬 MiniText.h**: Hiển thị văn bản và các thông báo trên màn hình.
    * **🖼️ SpriteComponent.h**: Quản lý hiển thị và animation của đối tượng, chuyển đổi hình ảnh theo từng frame.
    * **🧱 TileComponent.h**: Xử lý các ô (tiles) để tạo nên bản đồ game.
    * **📍 TransformComponent.h**: Quản lý vị trí, vận tốc và scale của đối tượng.

- **📚 src**: Folder chứa các thư viện cần thiết của SDL2, SDL_image, SDL_mixer, SDL_ttf.

- **📦 AssetManager.h/.cpp**: Quản lý tài nguyên game (âm thanh, hình ảnh, font chữ) với cơ chế load/unload hiệu quả.

- **💥 Collision.h/.cpp**: Cung cấp các hàm kiểm tra va chạm giữa các đối tượng trong game.

- **🎮 Game.h/.cpp**: 
    * File chính khởi tạo game engine, khởi tạo SDL, xử lý vòng lặp game.
    * Quản lý state game (menu, playing, pause), xử lý sự kiện, render hình ảnh, cập nhật entities.
    * Cơ chế PauseGame: Khi người chơi nhấn nút Pause, game sẽ hiển thị màn hình tạm dừng và tạm ngưng cập nhật trạng thái.

- **🎯 CrossyRoad.cpp**: File chính chứa hàm main(), điều khiển vòng lặp game và giới hạn FPS ở mức ~60 để đảm bảo trải nghiệm ổn định.

- **🚀 CrossyRoad.exe**: File thực thi đã được biên dịch, người dùng có thể chạy trực tiếp để chơi game.

- **🗺️ Map.h/.cpp**: Quản lý việc tải và hiển thị bản đồ từ file:
  ![Image](https://github.com/user-attachments/assets/b9b333a9-f594-411c-9d7d-0a31d5247346)
  
  Sử dụng tilemap system: Mỗi ô (tile) được đánh số từ 0 đến n, và được sắp xếp theo một file map (dạng text) định nghĩa cấu trúc của bản đồ:
  ![Image](https://github.com/user-attachments/assets/fbd10d43-27b0-4455-b740-22d34347970e)

- **🏆 ScoreSystem.h/.cpp**: Quản lý hệ thống điểm số, bao gồm:
    * Tính điểm theo tiến trình của người chơi
    * Lưu và hiển thị điểm cao nhất
    * Quản lý bảng xếp hạng top 10

- **➡️ Vector2D.h/.cpp**: Định nghĩa lớp Vector2D hỗ trợ các phép toán vector 2D (cộng, trừ, nhân, chia) để xử lý vị trí và chuyển động trong game.

# 📚 Các nguồn tham khảo 
## 💻 Code 
- [🎥 Nguồn từ YouTube](https://www.youtube.com/@CarlBirch)
- [📖 Nguồn từ trang web Lazyfool](https://lazyfoo.net/tutorials/SDL/index.php)
## 🎨 Đồ họa
- [🚗 Nguồn ảnh ô tô](https://minzinn.itch.io/pixelvehicles/devlog)
- [🐔 Nguồn ảnh gà](https://opengameart.org/content/chick)
- [🖌️ Nguồn ứng dụng vẽ pixel](https://www.aseprite.org/)
## 🔊 Âm thanh
- [🐔 Nguồn âm thanh tiếng gà kêu](https://uppbeat.io/sfx/category/birds/chicken)
- [🖱️ Nguồn âm thanh khi bấm chuột](https://mixkit.co/free-sound-effects/click/)
- [🎵 Nguồn nhạc nền](https://downloads.khinsider.com/game-soundtracks/album/crossy-road-castle-soundtrack-vol.-1-2-2020)
## 🔤 Phông chữ
- [📝 Nguồn phông chữ](https://www.fontspace.com/search?q=pixel)