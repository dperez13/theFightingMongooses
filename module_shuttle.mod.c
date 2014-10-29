#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0xca05c877, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0xda5a23c1, __VMLINUX_SYMBOL_STR(single_release) },
	{ 0xc3e1132a, __VMLINUX_SYMBOL_STR(seq_read) },
	{ 0xc916e9f3, __VMLINUX_SYMBOL_STR(kthread_stop) },
	{ 0xd122ae20, __VMLINUX_SYMBOL_STR(remove_proc_entry) },
	{ 0x8ca9f5d7, __VMLINUX_SYMBOL_STR(proc_create_data) },
	{ 0x59e6c6fe, __VMLINUX_SYMBOL_STR(wake_up_process) },
	{ 0xb3599333, __VMLINUX_SYMBOL_STR(kthread_create_on_node) },
	{ 0x49351084, __VMLINUX_SYMBOL_STR(STUB_issue_request) },
	{ 0xa1346a90, __VMLINUX_SYMBOL_STR(STUB_start_shuttle) },
	{ 0x750a30, __VMLINUX_SYMBOL_STR(STUB_stop_shuttle) },
	{ 0xf9a482f9, __VMLINUX_SYMBOL_STR(msleep) },
	{ 0xa8b94eb8, __VMLINUX_SYMBOL_STR(mutex_unlock) },
	{ 0xc09b4964, __VMLINUX_SYMBOL_STR(mutex_lock) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0xa05f372e, __VMLINUX_SYMBOL_STR(seq_printf) },
	{ 0x6fdd98fa, __VMLINUX_SYMBOL_STR(single_open) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "BF438237EC5DDE2D99977E8");
