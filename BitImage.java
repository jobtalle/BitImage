package utils;

import java.awt.image.BufferedImage;

public class BitImage {
	public static byte[] write(BufferedImage source, int designatedColor) {
		int pixelCount = source.getWidth() * source.getHeight();
		
		if(isMultipleOf2(pixelCount)) {
			int byteCount = pixelCount >> 3;
			byte[] bitImage = new byte[byteCount];
			
			for(int octet = 0; octet < byteCount; ++octet) {
				for(int bit = 0; bit < 8; ++bit) {
					int pixelIndex = (octet << 3) + bit;
					int pixel = source.getRGB(pixelIndex % source.getWidth(), pixelIndex / source.getWidth());
					
					if(pixel == designatedColor) bitImage[octet] |= (1 << bit);
				}
			}
			
			return bitImage;
		}
		
		return null;
	}
	
	public static BufferedImage read(byte[] source, int width, int height, int color0, int color1) {
		int pixelCount = width * height;
		
		if(isMultipleOf2(pixelCount)) {
			int byteCount = pixelCount >> 3;
			BufferedImage image = new BufferedImage(width, height, BufferedImage.TYPE_INT_ARGB);
			
			for(int octet = 0; octet < byteCount; ++octet) {
				for(int bit = 0; bit < 8; ++bit) {
					int pixelIndex = (octet << 3) + bit;
					
					if((source[octet] & (1 << bit)) == 0) {
						image.setRGB(pixelIndex % width, pixelIndex / width, color0);
					}
					else {
						image.setRGB(pixelIndex % width, pixelIndex / width, color1);
					}
				}
			}
			
			return image;
		}
		
		return null;
	}
	
	private static boolean isMultipleOf2(int n) {
		if((n & (n - 1)) == 0) return true;
		return false;
	}
}
