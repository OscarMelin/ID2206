#![feature(lang_items)]
#![feature(const_fn, unique)]
#![no_std]

extern crate rlibc;
extern crate volatile;
extern crate spin;

#[macro_use]
mod vga_buffer;

#[no_mangle]
pub extern "C" fn rust_main() {
    // ATTENTION: we have a very small stack and no guard page
    vga_buffer::clear_screen();
    println!("Hello World{}", "!");
    println!("{}", { println!("inner"); "outer" });

    loop{}
}

#[cfg(not(test))]
#[lang = "eh_personality"]
extern "C" fn eh_personality() {}

#[cfg(not(test))]
#[lang = "panic_fmt"]
extern "C" fn panic_fmt() -> ! {loop{}}

#[allow(non_snake_case)]
#[no_mangle]
pub extern "C" fn _Unwind_Resume() -> ! {
    loop {}
}
