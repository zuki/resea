# macで実行

ARCH = x86_64

```
$ make run

SeaBIOS (version rel-1.15.0-0-g2dd4b9b3f840-prebuilt.qemu.org)


iPXE (http://ipxe.org) 00:03.0 CA00 PCI2.10 PnP PMM+1FF91380+1FEF1380 CA00



Booting from ROM..

Booting Resea v0.9.0 (2bf14db)...
[kernel] Booted CPU #0
[vm] ready
shell> [dm] ready
[random] ready
[datetime] ready

[fatfs] bpb: magic=0xeb3c90
[fatfs] bpb: oem_name=MTOO4038, sector_size=512, sectors_per_cluster=2
[fatfs] bpb: num_reserved_sectors=1, num_fat=2, num_root_entries=512
[fatfs] bpb: num_total_sectors16=8196, media_id=248, sectors_per_fat16=12
[fatfs] bpb: sectors_per_track=63, num_head=16, hidden_sectors=0
[fatfs] bpb: num_total_sectors32=0, sectors_per_fat32=-1809252352, flags=47426
[fatfs] bpb: fat_version=20048, root_entries_cluster=1095639119, fsinfo_sector=7
[fatfs] bpb: sectors_per_backup_boot=8224, fsinfo_sector=17741, sectors_per_bacu
[fatfs] bpb: drive_number=192, winnt_flags=142, signature=216
[fatfs] bpb: volume_id=????, volume_label=, fat32_string=V
[fatfs] bpb: magic=0x55aa

[fatfs] sectors=8196, non_data_sectors=57, bpb.sectors_per_cluster=2, total_data_clus=4069  // Mac
[fatfs] sectors=8064, non_data_sectors=97, bpb.sectors_per_cluster=1, total_data_clus=7967  // ubuntu
[fatfs] WARN: FAT12 not supported
[fatfs] PANIC: failed to locate a FAT file system
[fatfs] WARN: Backtrace:
[fatfs] WARN:     #0: 0000000001005173 main()+0xe53
[fatfs] WARN:     #1: 0000000001001a47 resea_init()+0x57
[fatfs] WARN:     #2: 00000000010002c7 halt()+0x0
[vm] WARN: fatfs: exception occurred, killing the task...
[e1000] initialized the device
[e1000] MAC address = 52:54:00:12:34:56
[tcpip] ready
[tcpip] registered new net device 'net1'
[e1000] ready
[tcpip] WARN: retrying DHCP discover...
[tcpip] dhcp: leased ip=10.0.2.15, netmask=255.255.255.0, gateway=10.0.2.2
[tcpip] dhcp: leased ip=10.0.2.15, netmask=255.255.255.0, gateway=10.0.2.2
```

## servers/drivers/blk/ide/build.mkを修正でfatfsが動いた

```
mformat -i mformat -i $@.tmp -c 1       // -c 1 を追加

$ make run

[fatfs] bpb: magic=0xeb3c90
[fatfs] bpb: oem_name=MTOO4038, sector_size=512, sectors_per_cluster=1
[fatfs] bpb: num_reserved_sectors=1, num_fat=2, num_root_entries=512
[fatfs] bpb: num_total_sectors16=8196, media_id=248, sectors_per_fat16=32
[fatfs] bpb: sectors_per_track=63, num_head=16, hidden_sectors=0
[fatfs] bpb: num_total_sectors32=0, sectors_per_fat32=-517406720, flags=31137
[fatfs] bpb: fat_version=20026, root_entries_cluster=1095639119, fsinfo_sector=17741
[fatfs] bpb: sectors_per_backup_boot=8224, fsinfo_sector=17741, sectors_per_backup_boot=8224
[fatfs] bpb: drive_number=192, winnt_flags=142, signature=216
[fatfs] bpb: volume_id=????, volume_label=, fat32_string=V
[fatfs] bpb: magic=0x55aa
[fatfs] sectors=8196, non_data_sectors=97, bpb.sectors_per_cluster=1, total_data_clus=8099
[kernel] enabled IRQ: task=e1000, vector=11
[fatfs] Files ---------------------------------------------
[fatfs] /HELLO   TXT
[fatfs] ---------------------------------------------------
[fatfs] ready
[e1000] initialized the device
[e1000] MAC address = 52:54:00:12:34:56
[tcpip] starting...
[tcpip] ready
[tcpip] registered new net device 'net1'
[e1000] ready
[e1000] sent 302 bytes
[tcpip] WARN: retrying DHCP discover...
[e1000] sent 302 bytes
[e1000] received 316 bytes
[e1000] received 316 bytes
[e1000] sent 306 bytes
[e1000] received 316 bytes
[tcpip] dhcp: leased ip=10.0.2.15, netmask=255.255.255.0, gateway=10.0.2.2
[e1000] sent 306 bytes
[e1000] received 316 bytes
[tcpip] dhcp: leased ip=10.0.2.15, netmask=255.255.255.0, gateway=10.0.2.2      // ここでストール
```
