<p>
  OpenAI TTS service for the Freeswitch. <br>
</p>

### Usage example
```XML
<extension name="tts-test">
    <condition field="destination_number" expression="^(3333)$">
        <action application="answer"/>
        <action application="speak" data="openai|en|Hello world!"/>
        <action application="sleep" data="1000"/>
        <action application="hangup"/>
    </condition>
</extension>

```
