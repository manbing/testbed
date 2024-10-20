fn main() {
    let mut b: [u8; 4] = [0; 4];

    let char: char = '我';
    let result: &mut str = char.encode_utf8(&mut b);
    //println!("{:?}", b); // Decimal
    assert_eq!([230, 136, 145, 0], b);

    // reset
    for elem in b.iter_mut() {
        *elem = 0;
    }

    let char2: char = 'A';
    let result2 = char2.encode_utf8(&mut b);
    //println!("{:?}", b); // Decimal
    assert_eq!([65, 0, 0, 0], b);

    let str: &str = "Hello world";
    let result3 = str.as_bytes();
    //println!("{:#x?}", result3); // Hexadecimal
    assert_eq!([0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x20, 0x77, 0x6f, 0x72, 0x6c, 0x64], result3);
}
