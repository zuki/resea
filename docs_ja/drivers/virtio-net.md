# virtio-net

virtio-netバージョン1.x(*modern device*)とバージョン0.x (*legacy device*)用の
ネットワークデバイスドライバです。

## QEMUでの有効化の方法

```
$ make run VIRTIO_NET=1
$ make run VIRTIO_NET=1 VIRTIO_MODERN=1
```

## 参考資料

- [Virtual I/O Device (VIRTIO) Version 1.1](http://docs.oasis-open.org/virtio/virtio/v1.1/virtio-v1.1.html)

## ソースコード

[servers/drivers/net/virtio_net](https://github.com/zuki/resea/tree/master/servers/drivers/net/virtio_net)
