# ハードウェア・アシステッド・ハイパバイザ (hv)

## エミュレートするペリフェラルとデバイス

- シリアルポート
- PCI
  - virtio-blk (legacy)
- PIC (Intel 8259)
- PIT (Intel 8254)
  - *不完全: 設定に関わらず1msごとにIRQ #0を挿入する*
- APIC
  - *不完全: すべての書き込みを無視し、APIICローカルタイマーはサポートしていない*

## サポートするカーネルイメージフォーマット

- ELF
- ELF with [Xen PVH](https://github.com/xen-project/xen/blob/master/docs/misc/pvh.pandoc)

## テスト済みのゲストOS

- Resea
- Linux ([config](https://gist.github.com/nuta/e76ca295ebeec02b88121a1ae7c73b9e))

## hvの実行

1. ネストされたVMXを有効にしたLinuxを使用する
2. `CONFIG_HYPERVISOR`, `HV_SERVER`, `RAMDISK_SERVER`を有効にする
3. `VMX=y`オプションを付けて実行する

```
make run VMX=y HV_GUEST_IMAGE=vmlinux.elf RAM_DISK_IMG=rootfs.ext4.img
```


## デバッグの際のチップ（Linux KVM上）

### トレースメッセージの有効化

```
echo 1 | sudo tee /sys/kernel/tracing/events/kvm/kvm_nested_vmexit/enable
echo 1 | sudo tee /sys/kernel/tracing/events/kvm/kvm_nested_intr_vmexit/enable
echo 1 | sudo tee /sys/kernel/tracing/events/kvm/kvm_nested_vmexit/enable
echo 1 | sudo tee /sys/kernel/tracing/events/kvm/kvm_nested_vmenter_failed/enable
echo 1 | sudo tee /sys/kernel/debug/tracing/tracing_on
```

### トレースメッセージの読み込み

```
sudo watch tail /sys/kernel/debug/tracing/trace
```
