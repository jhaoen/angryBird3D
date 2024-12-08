以下說明把 Bullet 編譯並加入ogre專案的流程

1. 下載 Bullet : https://github.com/bulletphysics/bullet3/releases

2. 在下載的 Bullet 建立 build 資料夾

3. cd 至 build 資料夾，執行: cmake .. -DBUILD_BULLET2_LIB=ON -DBUILD_SHARED_LIBS=OFF -DBUILD_EXTRAS=OFF -DBUILD_UNIT_TESTS=OFF -DUSE_GLUT=OFF
(注意 -DBUILD_SHARED_LIBS=OFF 一定要是 OFF，不然無法編譯出 .lib)

4. 執行後會出現 Bullet 的 .sln，用 visual studio 開啟接著 build_all

5. 設置專案的包含路徑：
- 在 Visual Studio 中，右鍵單擊你的專案(OgreBasicApp)，選擇 屬性。
- 在 配置屬性 -> C/C++ -> General -> Additional Include Directories 中，添加 Bullet src 的路徑

6. 添加依賴的 .lib 文件：
在 配置屬性 -> Linker -> Input -> Additional Dependencies 中，添加以下庫文件：
- LinearMath.lib  
- BulletCollision.lib  
- BulletDynamics.lib  

7. 把 VS 的設置改成 static linking
- 在 Visual Studio 中，右鍵單擊你的專案，選擇 屬性。
- 在 Configuration Properties -> C/C++ -> Code Generation -> Runtime Library 中：把 Multi-threaded (/MD) 改成 Multi-threaded (/MT)

8. 最後 Build 整個專案，應該是可以執行