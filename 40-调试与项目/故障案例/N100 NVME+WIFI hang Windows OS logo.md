
N100 4L项目存在两个设备共享同一PCIe时钟的情况

![[N100 NVME+WIFI hang Windows OS logo-01.png]]

![[N100 NVME+WIFI hang Windows OS logo-02.png]]

两个PCIe设备用同一个clock的时候，要设置clock时钟自动输出
否则的话NVME设备可能和Intel wifi设备冲突，hang在Windows logo的地方