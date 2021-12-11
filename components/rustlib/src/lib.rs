#![cfg_attr(not(feature = "std"), no_std)]
#![feature(asm)]

#[cfg(not(feature = "std"))]
use core::panic::PanicInfo;

use std::ffi::CStr;
// use std::ffi::CString;
use std::os::raw::c_char;
// use std::error::Error;

use std::fs::File;
use std::io::Write;

/// Create aliases for FFI types for esp32c3, which doesn't have std.
#[cfg(not(feature = "std"))]
mod ffi {
    #![allow(dead_code)]
    #![allow(non_upper_case_globals)]
    #![allow(non_camel_case_types)]
    #![allow(non_snake_case)]

    pub type c_char = u8;
    pub type c_int = i32;
}

pub mod sys {
    #![allow(non_upper_case_globals)]
    #![allow(non_camel_case_types)]
    #![allow(non_snake_case)]
    include!(concat!(env!("OUT_DIR"), "/bindings.rs"));
}

#[no_mangle]
pub extern "C" fn create_file(path: *const c_char) -> i32 {
    let path_ok;
    unsafe {
        path_ok = CStr::from_ptr(path);
    }
    println!("Hello from rust\n, file: {:?} will be created", path_ok);
    let mut ofile = File::create(path_ok.to_str().unwrap_or("slt")).expect("unable to create file");
    // ofile.write_all(output.as_bytes()).expect("unable to write");
    5
}

#[cfg(not(feature = "std"))]
#[panic_handler]
fn panic(_info: &PanicInfo) -> ! {
    loop {}
}
