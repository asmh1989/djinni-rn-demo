./build_ios/libstml.xcodeproj: libstml.gyp ./deps/djinni/support-lib/support_lib.gyp djinni/stml.djinni
	sh ./run_djinni.sh
	deps/gyp/gyp --depth=. -f xcode -DOS=ios --generator-output ./build_ios -Ideps/djinni/common.gypi ./libstml.gyp

ios: ./build_ios/libstml.xcodeproj
	xcodebuild -workspace ios/RNSmobiler.xcworkspace \
		-scheme RNSmobiler \
		-configuration 'Debug' \
		-sdk iphonesimulator

GypAndroid.mk: libstml.gyp ./deps/djinni/support-lib/support_lib.gyp djinni/stml.djinni
	sh ./run_djinni.sh
	ANDROID_BUILD_TOP=$(shell dirname `which ndk-build`) deps/gyp/gyp --depth=. -f android -DOS=android -Ideps/djinni/common.gypi ./libtodoapp.gyp --root-target=libtodoapp_jni

android: GypAndroid.mk
	cd android && ./gradlew app:assembleDebug
	@echo "Apks produced at:"
	@python deps/djinni/example/glob.py ./ '*.apk'

library: ./build_ios/libstml.xcodeproj

clean:
	rm -rf ./build_ios ./generated-src .*~ src/.*~
